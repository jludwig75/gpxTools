#include "calculator.h"


#include "gpxmath.h"


using grpc::Status;
using grpc::StatusCode;

using namespace gpx;


grpc::Status GpxCalculatorImpl::analyzeTrack(grpc::ServerContext* context,
                    const gpxtools::Track* track,
                    grpc::ServerWriter<gpxtools::DataPoint>* writer)
{
    if (track->tracksegments_size() == 0 || track->tracksegments(0).trackpoints_size() < 2)
    {
        return Status(grpc::StatusCode::INVALID_ARGUMENT, "no track points");
    }

    const gpxtools::TrackPoint* start = NULL;
    const gpxtools::TrackPoint* previous = NULL;
    double totalDistance = 0;

    for (const auto& segment : track->tracksegments())
    {
        for (const auto& point : segment.trackpoints())
        {
            if (start == NULL)
            {
                start = &point;
            }

            if (previous != NULL)
            {
                assert(point.time() >= previous->time());

                auto displacement = point.position() - previous->position();
                totalDistance += displacement.horizontal;
                gpxtools::DataPoint dataPoint;
                dataPoint.set_relstarttime(point.time() - start->time());
                dataPoint.set_duration(point.time() - previous->time());
                dataPoint.set_totaldistance(totalDistance);
                dataPoint.set_altitude(point.position().altitude());
                dataPoint.set_horizontaldisplacement(displacement.horizontal);
                dataPoint.set_verticaldisplacement(displacement.vertical);
                writer->Write(dataPoint);
            }

            previous = &point;
        }
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
