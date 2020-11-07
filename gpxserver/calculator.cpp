#include "calculator.h"


#include "gpxmath.h"


using grpc::Status;
using grpc::StatusCode;

using namespace gpx;


grpc::Status GpxCalculatorImpl::analyzeTrack(grpc::ServerContext* context,
                        grpc::ServerReaderWriter<gpxtools::DataPoint, gpxtools::TrackPoint>* stream)
{
    int64_t startTime = INT16_MAX;
    gpxtools::TrackPoint previous;
    double totalDistance = 0;

    gpxtools::TrackPoint point;
    while (stream->Read(&point))
    {
        if (startTime == INT16_MAX)
        {
            startTime = point.time();
        }
        else
        {
            assert(point.time() >= previous.time());

            auto displacement = point.position() - previous.position();
            totalDistance += displacement.horizontal;
            gpxtools::DataPoint dataPoint;
            dataPoint.set_relstarttime(point.time() - startTime);
            dataPoint.set_duration(point.time() - previous.time());
            dataPoint.set_totaldistance(totalDistance);
            dataPoint.set_altitude(point.position().altitude());
            dataPoint.set_horizontaldisplacement(displacement.horizontal);
            dataPoint.set_verticaldisplacement(displacement.vertical);
            stream->Write(dataPoint);
        }

        previous = point;
    }

    return Status::OK;
}

grpc::Status GpxCalculatorImpl::summarizeStream(grpc::ServerContext* context,
                        grpc::ServerReader<gpxtools::DataPoint>* reader,
                        gpxtools::DataSummary* summary)
{
    double totalAscent = 0;
    double totalDescent = 0;

    gpxtools::DataPoint dataPoint;
    bool dataPointsRead = false;
    int64_t startTime = -1;
    while (reader->Read(&dataPoint))
    {
        dataPointsRead = true;
        if (startTime == -1)
        {
            startTime = dataPoint.relstarttime();
        }
        if (dataPoint.verticaldisplacement() > 0)
        {
            totalAscent += dataPoint.verticaldisplacement();
        }
        else if (dataPoint.verticaldisplacement() < 0)
        {
            totalDescent += -dataPoint.verticaldisplacement();
        }
    }

    if (dataPointsRead)
    {
        auto totalDistance = dataPoint.totaldistance();
        auto elapsedTime = dataPoint.relstarttime() - startTime;

        summary->set_duration(elapsedTime);
        summary->set_totaldistance(totalDistance);
        summary->set_totalascent(totalAscent);
        summary->set_totaldescent(totalDescent);
        summary->set_averagespeed(elapsedTime == 0 ? 0 : totalDistance / elapsedTime);
        return Status::OK;
    }

    return Status(StatusCode::INVALID_ARGUMENT, "Empty data point stream");
}
