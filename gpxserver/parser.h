#pragma once

#include <grpc/grpc.h>
#include <grpcpp/server.h>

#include "gpxtools.grpc.pb.h"


class ParserImpl final : public gpxtools::Parser::Service
{
public:
    ParserImpl();
    ~ParserImpl();
    grpc::Status parseFile(grpc::ServerContext* context,
                           grpc::ServerReader<gpxtools::GpxDataChunk>* reader,
                           gpxtools::Activity* activity) override;
};