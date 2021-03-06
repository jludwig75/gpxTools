#include "parser.h"

#include <grpcpp/impl/codegen/status_code_enum.h>

#include <iomanip>
#include <limits>
#include <memory>
#include <sstream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/framework/MemBufInputSource.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
 
using namespace xercesc;


using grpc::Status;
using grpc::StatusCode;

namespace internal
{
bool parseFile(const std::string& gpxFileData, grpc::ServerReaderWriter<gpxtools::TrackPoint, gpxtools::GpxDataChunk>* stream);
}

ParserImpl::ParserImpl()
{
    XMLPlatformUtils::Initialize();
}

ParserImpl::~ParserImpl()
{
    XMLPlatformUtils::Terminate();
}

grpc::Status ParserImpl::parseFile(grpc::ServerContext* context,
                        grpc::ServerReaderWriter<gpxtools::TrackPoint, gpxtools::GpxDataChunk>* stream)
{
    std::string gpxData;

    gpxtools::GpxDataChunk chunk;

    while (stream->Read(&chunk))
    {
        gpxData += chunk.data();
    }

    if (!internal::parseFile(gpxData, stream))
    {
        return Status(StatusCode::INVALID_ARGUMENT, "Invalid GPX data");
    }

    return Status::OK;
}


namespace internal
{

template<typename ValueType>
ValueType fromString(const std::string& str)
{
    std::stringstream ss(str);
    ValueType v;
    ss >> v;
    assert(!ss.bad() && ss.eof());
    return v;
}

std::time_t parseTimeString(const std::string& timeStr)
{
    std::tm tm = {};
    std::stringstream ss(timeStr);
    // 2020-10-31T16:35:15Z
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return timegm(&tm);
}


bool parseTrackPoint(DOMElement* trackPointElement, grpc::ServerReaderWriter<gpxtools::TrackPoint, gpxtools::GpxDataChunk>* stream, bool startOfSegment)
{
    double latitude = std::numeric_limits<double>::infinity();
    double longitude = std::numeric_limits<double>::infinity();
    double altitude = std::numeric_limits<double>::infinity();
    std::time_t time = 0;

    auto TAG_lat = XMLString::transcode("lat");
    auto TAG_lon = XMLString::transcode("lon");
    auto TAG_ele = XMLString::transcode("ele");
    auto TAG_time = XMLString::transcode("time");

    DOMNodeList* trackPointChildren = trackPointElement->getChildNodes();
    const  XMLSize_t trackPointChildrenCount = trackPointChildren->getLength();
    for( XMLSize_t t = 0; t < trackPointChildrenCount; ++t )
    {
        DOMNode* trackPointChildNode = trackPointChildren->item(t);
        if( trackPointChildNode->getNodeType())  // true is not NULL
        {
            auto nodeType = trackPointChildNode->getNodeType();
            if (trackPointChildNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
            {
                // Found node which is an Element. Re-cast node as element
                DOMElement* trackSegmentChildElement
                            = dynamic_cast< xercesc::DOMElement* >( trackPointChildNode );
                if( XMLString::equals(trackSegmentChildElement->getTagName(), TAG_ele))
                {
                    DOMNode *node = trackSegmentChildElement->getFirstChild();
                    std::string value = XMLString::transcode(node->getNodeValue());
                    altitude = fromString<double>(value);
                }
                else if( XMLString::equals(trackSegmentChildElement->getTagName(), TAG_time))
                {
                    DOMNode *node = trackSegmentChildElement->getFirstChild();
                    std::string timeStr = XMLString::transcode(node->getNodeValue());
                    time = parseTimeString(timeStr);
                }
            }
        }
    }

    auto* attrs = trackPointElement->getAttributes();

    auto* latAttr = attrs->getNamedItem(TAG_lat);
    assert(latAttr != NULL);
    std::string latStr = XMLString::transcode(latAttr->getNodeValue());
    latitude = fromString<double>(latStr);

    auto* lonAttr = attrs->getNamedItem(TAG_lon);
    assert(lonAttr != NULL);
    std::string lonStr = XMLString::transcode(lonAttr->getNodeValue());
    longitude = fromString<double>(lonStr);

    assert(latitude != std::numeric_limits<double>::infinity() &&
            longitude != std::numeric_limits<double>::infinity() &&
            altitude != std::numeric_limits<double>::infinity() &&
            time != 0);
    gpxtools::SurfaceCoordinates coords;
    coords.set_latitude(latitude);
    coords.set_longitude(longitude);
    gpxtools::Position position;
    *position.mutable_coords() = coords;
    position.set_altitude(altitude);
    gpxtools::TrackPoint trackPoint;
    *trackPoint.mutable_position() = position;
    trackPoint.set_time(time);
    trackPoint.set_start_new_segment(startOfSegment);
    return stream->Write(std::move(trackPoint));
}

bool parseTrackSegment(DOMElement* trackSegmentElement, grpc::ServerReaderWriter<gpxtools::TrackPoint, gpxtools::GpxDataChunk>* stream)
{
    auto TAG_trackPt = XMLString::transcode("trkpt");

    bool startOfSegment = true;
    DOMNodeList* trackSegmentChildren = trackSegmentElement->getChildNodes();
    const  XMLSize_t trackSgementChildrenCount = trackSegmentChildren->getLength();
    for( XMLSize_t t = 0; t < trackSgementChildrenCount; ++t )
    {
        DOMNode* trackSegmentChildNode = trackSegmentChildren->item(t);
        if( trackSegmentChildNode->getNodeType() &&  // true is not NULL
            trackSegmentChildNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
        {
            // Found node which is an Element. Re-cast node as element
            DOMElement* trackSegmentChildElement
                        = dynamic_cast< xercesc::DOMElement* >( trackSegmentChildNode );
            if( XMLString::equals(trackSegmentChildElement->getTagName(), TAG_trackPt))
            {
                if (!parseTrackPoint(trackSegmentChildElement, stream, startOfSegment))
                {
                    return false;
                }
                startOfSegment = false;
            }
        }
    }

    return true;
}

bool parseTrack(DOMElement* trackElement, grpc::ServerReaderWriter<gpxtools::TrackPoint, gpxtools::GpxDataChunk>* stream)
{
    auto TAG_trackSeg = XMLString::transcode("trkseg");
    auto TAG_name = XMLString::transcode("name");
    auto TAG_type = XMLString::transcode("type");

    std::string trackName = "";
    unsigned trackType = UINT_MAX;

    DOMNodeList* trackChildren = trackElement->getChildNodes();
    const  XMLSize_t trackCount = trackChildren->getLength();
    for( XMLSize_t t = 0; t < trackCount; ++t )
    {
        DOMNode* trackChildNode = trackChildren->item(t);
        if( trackChildNode->getNodeType() &&  // true is not NULL
            trackChildNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
        {
            // Found node which is an Element. Re-cast node as element
            DOMElement* trackChildElement
                        = dynamic_cast< xercesc::DOMElement* >( trackChildNode );
            if( XMLString::equals(trackChildElement->getTagName(), TAG_trackSeg))
            {
                if (!parseTrackSegment(trackChildElement, stream))
                {
                    return false;
                }
            }
            else if( XMLString::equals(trackChildElement->getTagName(), TAG_name))
            {
                DOMNode *node = trackChildElement->getFirstChild();
                trackName = XMLString::transcode(node->getNodeValue());
            }
            else if( XMLString::equals(trackChildElement->getTagName(), TAG_type))
            {
                DOMNode *node = trackChildElement->getFirstChild();
                std::string value = XMLString::transcode(node->getNodeValue());
                trackType = fromString<unsigned>(value);
            }
        }
    }

    // TODO: replace this
    // gpxtools::Track track;
    // track.set_name(trackName);
    // track.set_type(trackType);

    return true;
}   // namespace

bool parseFile(const std::string& gpxFileData, grpc::ServerReaderWriter<gpxtools::TrackPoint, gpxtools::GpxDataChunk>* stream)
{
    auto TAG_root = XMLString::transcode("gpx");
    auto TAG_track = XMLString::transcode("trk");
    auto TAG_metadata = XMLString::transcode("metadata");
    auto TAG_time = XMLString::transcode("time");

    auto parser = std::make_unique<XercesDOMParser>();
    // Configure DOM parser.
    parser->setValidationScheme( XercesDOMParser::Val_Never );
    parser->setDoNamespaces( false );
    parser->setDoSchema( false );
    parser->setLoadExternalDTD( false );

    MemBufInputSource dataSource((const XMLByte*)gpxFileData.data(), gpxFileData.length(), "");
    parser->parse(dataSource);

    auto xmlDoc = parser->getDocument();
    auto elementRoot = xmlDoc->getDocumentElement();
    DOMNodeList* children = elementRoot->getChildNodes();
    const  XMLSize_t nodeCount = children->getLength();

    // For all nodes, children of "root" in the XML tree.
    for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
    {
        DOMNode* currentNode = children->item(xx);
        if( currentNode->getNodeType() &&  // true is not NULL
            currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
        {
            // Found node which is an Element. Re-cast node as element
            DOMElement* currentElement
                        = dynamic_cast< xercesc::DOMElement* >( currentNode );
            if( XMLString::equals(currentElement->getTagName(), TAG_track))
            {
                if (!parseTrack(currentElement, stream))
                {
                    return false;
                }
            }
            else if( XMLString::equals(currentElement->getTagName(), TAG_metadata))
            {
                DOMNodeList* metadatChildren = currentElement->getChildNodes();
                const  XMLSize_t metadatChildrenCount = metadatChildren->getLength();
                for( XMLSize_t t = 0; t < metadatChildrenCount; ++t )
                {
                    DOMNode* metadatChildNode = metadatChildren->item(t);
                    if( metadatChildNode->getNodeType())  // true is not NULL
                    {
                        if (metadatChildNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
                        {
                            // Found node which is an Element. Re-cast node as element
                            DOMElement* metadatChildElement
                                        = dynamic_cast< xercesc::DOMElement* >( metadatChildNode );
                            if( XMLString::equals(metadatChildElement->getTagName(), TAG_time))
                            {
                                DOMNode *node = metadatChildElement->getFirstChild();
                                std::string timeStr = XMLString::transcode(node->getNodeValue());
                                std::time_t time = parseTimeString(timeStr);
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

} // namespace internal
