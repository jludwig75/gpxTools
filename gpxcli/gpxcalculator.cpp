#include "gpxcalculator.h"

#include <grpcpp/client_context.h>

#include <thread>

using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

GpxCalculator::GpxCalculator(std::shared_ptr<grpc::Channel> channel)
    :
    stub_(gpxtools::GpxCalculator::NewStub(channel))
{
}

bool GpxCalculator::analyzeTrack(Container<gpxtools::TrackPoint>& trackPoints, DataStream& dataStream)
{
    ClientContext context;
    std::unique_ptr<grpc::ClientReaderWriter<gpxtools::TrackPoint, gpxtools::DataPoint> > stream(stub_->analyzeTrack(&context));

    std::thread writer([&stream, &trackPoints]() {
        gpxtools::TrackPoint trackPoint;
        while (trackPoints.pop(trackPoint))
        {
            if (!stream->Write(trackPoint))
            {
                return false;
            }
        }
        stream->WritesDone();
    });

    gpxtools::DataPoint dataPoint;
    while (stream->Read(&dataPoint))
    {
        dataStream.push(dataPoint);
    }
    dataStream.done_pushing();

    writer.join();
    Status status = stream->Finish();
    return status.ok();
}

bool GpxCalculator::summarizeStream(DataStream& dataStream, gpxtools::DataSummary &dataSummary)
{
    ClientContext context;
    std::unique_ptr<ClientWriter<gpxtools::DataPoint> > writer(stub_->summarizeStream(&context, &dataSummary));

    gpxtools::DataPoint dataPoint;
    while (dataStream.pop(dataPoint))
    {
        if (!writer->Write(dataPoint))
        {
            std::cout << "Failed to write data point\n";
            return false;
        }
    }
    writer->WritesDone();

    Status status = writer->Finish();
    return status.ok();
}
