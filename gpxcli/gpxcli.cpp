#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "gpxtools.grpc.pb.h"

#include "conatiner.h"

#include "gpxparser.h"
#include "gpxcalculator.h"


using grpc::Channel;


template<typename ValueType>
ValueType mps_to_kph(ValueType mps)
{
    return mps * 60.0 * 60.0 / 1000.0;
}

template<typename ValueType>
ValueType mps_to_mph(ValueType mps)
{
    return mps * 60.0 * 60.0 * 3.28084 / 5280.0;
}

template<typename ValueType>
ValueType m_to_miles(ValueType m)
{
    return m * 3.28084 / 5280.0;
}

template<typename ValueType>
ValueType m_to_ft(ValueType m)
{
    return m * 3.28084;
}


std::string readFile(const std::string& fileName)
{
    std::ifstream t(fileName);
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    return str;
}

int parseGpxFile(const std::string& gpxFileName)
{
    GpxParser parser(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    auto gpxData = readFile(gpxFileName);

    GpxParser::TrackPoints trackPoints;
    parser.parseFile(gpxData, trackPoints);

    std::cout << "Parsed " << trackPoints.size() << " track points\n";
    return 0;
}

int plotStats(const std::string& gpxFileName)
{
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    GpxParser parser(channel);
    GpxCalculator calculator(channel);

    GpxParser::TrackPoints trackPoints;
    std::thread parserThread([gpxFileName, &parser, &trackPoints](){
        auto gpxData = readFile(gpxFileName);
        parser.parseFile(gpxData, trackPoints);
    });

    GpxCalculator::DataStream dataStream;
    std::thread analyzeThread([&calculator, &trackPoints, &dataStream](){
        calculator.analyzeTrack(trackPoints, dataStream);
    });

    std::cout << "time,altitude (meters),grade,speed (kph)\n";
    gpxtools::DataPoint dp;
    while (dataStream.pop(dp))
    {
        std::cout << dp.relstarttime() << "," << dp.totaldistance() << "," << dp.altitude() << "," << 100 * dp.grade() << "," << mps_to_kph(dp.speed()) << "\n";
    }

    parserThread.join();
    analyzeThread.join();

    return 0;
}

int summarize(const std::string& gpxFileName)
{
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    GpxParser parser(channel);
    GpxCalculator calculator(channel);

    GpxParser::TrackPoints trackPoints;
    std::thread parserThread([gpxFileName, &parser, &trackPoints](){
        auto gpxData = readFile(gpxFileName);
        parser.parseFile(gpxData, trackPoints);
    });

    GpxCalculator::DataStream dataStream;
    std::thread analyzeThread([&calculator, &trackPoints, &dataStream](){
        calculator.analyzeTrack(trackPoints, dataStream);
    });

    gpxtools::DataSummary summary;
    if (!calculator.summarizeStream(dataStream, summary))
    {
        std::cout << "Summarize stream failed\n";
        return -1;
    }

    auto elapsedTime = summary.duration();
    std::cout << "Totalt time: " << uint64_t(elapsedTime) / 3600 << ":" << (uint64_t(elapsedTime) % 3600) / 60 << ":" << ((uint64_t(elapsedTime) % 3600) % 60) << "\n";
    std::cout << "Totalt distance: " << summary.totaldistance() / 1000.0 << " km " << m_to_miles(summary.totaldistance()) << " miles\n";
    std::cout << "Average speed: " << mps_to_kph(summary.totaldistance() / elapsedTime) << " km/h " << mps_to_mph(summary.totaldistance() / elapsedTime) << "mph\n";
    std::cout << "Total Ascent: " << summary.totalascent() << " meters " << m_to_ft(summary.totalascent()) << " feet\n";
    std::cout << "Total Descent: " << summary.totaldescent() << " meters " << m_to_ft(summary.totaldescent()) << " feet\n";

    parserThread.join();
    analyzeThread.join();

    return 0;
}

int usage(const std::string& message, int ec = 0)
{
    if (!message.empty())
    {
        std::cout << message << std::endl << std::endl;
    }

    std::cout << "GPX utility\nUsage:\n\tgpx <command> <args ...>\n";
    std::cout << "Comamnd can be:\n";
    std::cout << "\tparse <gpx_file_name>\n";
    std::cout << "\tplot <gpx_file_name>\n";
    std::cout << "\tsummarize <gpx_file_name>\n";
    return ec;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty())
    {
        return usage("You must specify a command", -1);
    }

    auto command = args[0];

    if (command == "parse")
    {
        if (args.size() < 2)
        {
            return usage("\"parse\" command expects a file name");
        }

        return parseGpxFile(args[1]);
    }
    else if (command == "plot")
    {
        if (args.size() < 2)
        {
            return usage("\"plot\" command expects a file name");
        }

        return plotStats(args[1]);
    }
    else if (command == "summarize")
    {
        if (args.size() < 2)
        {
            return usage("\"summarize\" command expects a file name");
        }

        return summarize(args[1]);
    }
    else
    {
        return usage("Invalid command \"" + command + "\"");
    }
    

    return 0;
}
