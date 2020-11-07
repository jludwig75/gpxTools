#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/impl/codegen/status_code_enum.h>

#include "gpxtools.grpc.pb.h"

#include "gpxmath.h"
#include "parser.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
//using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using namespace gpx;

class ParserImpl final : public gpxtools::Parser::Service
{
public:
    Status parseFile(ServerContext* context,
                           ServerReader<gpxtools::GpxDataChunk>* reader,
                           gpxtools::Activity* activity) override
    {
        std::string gpxData;

        gpxtools::GpxDataChunk chunk;

        while (reader->Read(&chunk))
        {
            gpxData += chunk.data();
        }

        if (!_parser.parseFile(gpxData, activity))
        {
            return Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid GPX data");
        }

        return Status::OK;
    }
private:
    gpx::Parser _parser;
};

class GpxCalculatorImpl final : public gpxtools::GpxCalculator::Service
{
public:
    Status analyzeTrack(ServerContext* context,
                        const gpxtools::Track* track,
                        ServerWriter<gpxtools::DataPoint>* writer) override
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

    Status summarizeStream(ServerContext* context,
                           ServerReader<gpxtools::DataPoint>* reader,
                           gpxtools::DataSummary* summary) override
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

        return Status(grpc::StatusCode::INVALID_ARGUMENT, "Empty data point stream");
    }
};


void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GpxCalculatorImpl service;
  ParserImpl parser;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&parser);
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char* argv[])
{
    RunServer();
    return 0;
}
