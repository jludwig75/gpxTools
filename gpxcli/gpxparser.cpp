#include "gpxparser.h"

#include <grpcpp/client_context.h>

#include <thread>


#define MAX_DATA_CHUNCK_SIZE    (64 * 1024)


using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;


GpxParser::GpxParser(std::shared_ptr<grpc::Channel> channel)
    :
    stub_(gpxtools::Parser::NewStub(channel))
{
}

bool GpxParser::parseFile(const std::string& gpxFileData, TrackPoints& trackPoints)
{
    ClientContext context;
    std::unique_ptr<grpc::ClientReaderWriter<gpxtools::GpxDataChunk, gpxtools::TrackPoint> > stream(stub_->parseFile(&context));

    std::thread writer([&stream, &gpxFileData]() {
        std::string::size_type offset = 0;
        while (offset < gpxFileData.length())
        {
            gpxtools::GpxDataChunk chunk;
            std::string chunkData = gpxFileData.substr(offset, MAX_DATA_CHUNCK_SIZE);
            offset += chunkData.length();
            chunk.set_data(chunkData);
            if (!stream->Write(chunk))
            {
                // TODO: Handle error.
                return;
            }
        }
        stream->WritesDone();
    });

    gpxtools::TrackPoint trackPoint;
    while (stream->Read(&trackPoint))
    {
        trackPoints.push(trackPoint);
    }
    trackPoints.done_pushing();

    writer.join();
    Status status = stream->Finish();
    return status.ok();
}
