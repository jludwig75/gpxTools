#include "parser.h"

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

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
 
using namespace xercesc;

namespace gpx
{

namespace
{

template<typename Value>
Value fromString(const std::string& str)
{
    std::stringstream ss(str);
    Value v;
    ss >> v;
    assert(!ss.bad() && ss.eof());
    return v;
}

TrackPoint parseTrackPoint(DOMElement* trackPointElement)
{
    // TODO: parse this from XML
    double latitude = std::numeric_limits<double>::infinity();
    double longitude = std::numeric_limits<double>::infinity();
    double altitude = std::numeric_limits<double>::infinity();
    std::time_t time = 0;
    std::string timeStr;

    auto TAG_lat = XMLString::transcode("lat");
    auto TAG_lon = XMLString::transcode("lon");
    auto TAG_ele = XMLString::transcode("ele");
    auto TAG_time = XMLString::transcode("time");

    DOMNodeList*      trackPointChildren = trackPointElement->getChildNodes();
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
                    timeStr = XMLString::transcode(node->getNodeValue());
                    std::tm tm = {};
                    std::stringstream ss(timeStr);
                    // 2020-10-31T16:35:15Z
                    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
                    time = timegm(&tm);
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
    return TrackPoint(std::move(Position(std::move(SurfaceCoordinates(latitude, longitude)), altitude)), time);
}

TrackSegment parseTrackSegment(DOMElement* trackSegmentElement)
{
    auto TAG_trackPt = XMLString::transcode("trkpt");

    TrackSegment segment;

    DOMNodeList*      trackSegmentChildren = trackSegmentElement->getChildNodes();
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
                segment.addTrackPoint(std::move(parseTrackPoint(trackSegmentChildElement)));
            }
        }
    }

    return segment;
}

Track parseTrack(DOMElement* trackElement)
{
    auto TAG_trackSeg = XMLString::transcode("trkseg");
    auto TAG_name = XMLString::transcode("name");
    auto TAG_type = XMLString::transcode("type");

    std::string trackName = "";
    unsigned trackType = UINT_MAX;
    DOMElement* foundTrackChildElement = NULL;

    DOMNodeList*      trackChildren = trackElement->getChildNodes();
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
                foundTrackChildElement = trackChildElement;
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

    Track track(trackName, trackType);
    assert(foundTrackChildElement != NULL);
    track.addTrackSegment(std::move(parseTrackSegment(foundTrackChildElement)));

    return track;
}   // namespace

class XercesPlatformInit
{
public:
    XercesPlatformInit()
    {
        XMLPlatformUtils::Initialize();
    }
    ~XercesPlatformInit()
    {
        XMLPlatformUtils::Terminate();
    }
};

}

Activity parseFile(const std::string& gpxFileName)
{
    XercesPlatformInit platform;

    auto TAG_root = XMLString::transcode("gpx");
    auto TAG_track = XMLString::transcode("trk");

    auto parser = std::make_unique<XercesDOMParser>();
    // Configure DOM parser.
    parser->setValidationScheme( XercesDOMParser::Val_Never );
    parser->setDoNamespaces( false );
    parser->setDoSchema( false );
    parser->setLoadExternalDTD( false );

    parser->parse(gpxFileName.c_str());

    auto xmlDoc = parser->getDocument();
    auto elementRoot = xmlDoc->getDocumentElement();
    DOMNodeList*      children = elementRoot->getChildNodes();
    const  XMLSize_t nodeCount = children->getLength();

    Activity activity;

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
                activity.addTrack(std::move(parseTrack(currentElement)));
            }
        }
    }

    return activity;
}

}   // namepsace gpx