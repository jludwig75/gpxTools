#pragma once

#include <grpc/grpc.h>
#include <grpcpp/channel.h>

#include "conatiner.h"

#include "gpxtools.grpc.pb.h"


class GpxCalculator
{
public:
    using DataStream = Container<gpxtools::DataPoint>;
    GpxCalculator(std::shared_ptr<grpc::Channel> channel);
    bool analyzeTrack(Container<gpxtools::TrackPoint>& trackPoints, DataStream& dataStream);
    bool summarizeStream(DataStream& dataStream, gpxtools::DataSummary &dataSummary);
private:
    std::unique_ptr<gpxtools::GpxCalculator::Stub> stub_;
};
