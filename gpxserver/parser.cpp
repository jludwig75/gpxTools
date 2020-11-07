#include "parser.h"

#include <grpcpp/impl/codegen/status_code_enum.h>


using grpc::Status;
using grpc::StatusCode;


grpc::Status ParserImpl::parseFile(grpc::ServerContext* context,
                        grpc::ServerReader<gpxtools::GpxDataChunk>* reader,
                        gpxtools::Activity* activity)
{
    std::string gpxData;

    gpxtools::GpxDataChunk chunk;

    while (reader->Read(&chunk))
    {
        gpxData += chunk.data();
    }

    if (!_parser.parseFile(gpxData, activity))
    {
        return Status(StatusCode::INVALID_ARGUMENT, "Invalid GPX data");
    }

    return Status::OK;
}
