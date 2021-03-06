#pragma once

#include <grpc/grpc.h>
#include <grpcpp/server.h>

#include "gpxtools.grpc.pb.h"


class GpxCalculatorImpl final : public gpxtools::GpxCalculator::Service
{
public:
    grpc::Status analyzeTrack(grpc::ServerContext* context,
                        grpc::ServerReaderWriter<gpxtools::DataPoint, gpxtools::TrackPoint>* stream) override;

    grpc::Status summarizeStream(grpc::ServerContext* context,
                           grpc::ServerReader<gpxtools::DataPoint>* reader,
                           gpxtools::DataSummary* summary) override;
};