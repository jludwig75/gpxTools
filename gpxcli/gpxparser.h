#pragma once

#include <grpc/grpc.h>
#include <grpcpp/channel.h>

#include "conatiner.h"

#include "gpxtools.grpc.pb.h"

class GpxParser
{
public:
    using TrackPoints=Container<gpxtools::TrackPoint>;
    GpxParser(std::shared_ptr<grpc::Channel> channel);
    bool parseFile(const std::string& gpxFileData, TrackPoints& trackPoints);
private:
    std::unique_ptr<gpxtools::Parser::Stub> stub_;
};
