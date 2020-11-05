#include <iostream>
#include <string>
#include <vector>

#include "gpx/parser.h"
#include "gpx/gpxcalc.h"
#include "gpx/gpxmath.h"

using namespace gpx;

int parseGpxFile(const std::string& gpxFileName)
{
    Parser parser;
    auto activity = parser.parseFile(gpxFileName);
    std::cout << "Prased " << activity.tracks()[0].trackSegments()[0].trackPoints().size() << " track points\n";
    return 0;
}

int plotStats(const std::string& gpxFileName)
{
    Parser parser;
    auto activity = parser.parseFile(gpxFileName);
    std::cout << "time,altitude (meters),grade,speed (kph),rate of climb (m/s)\n";
    GpxCalculator calculator;
    for (const auto& track : activity.tracks())
    {
        auto dataStream = calculator.analyzeTrack(track);
        for (const auto& dp : dataStream)
        {
            std::cout << dp.relStartTime << "," << dp.totalDistance << "," << dp.altitude << "," << 100 * dp.grade() << "," << mps_to_kph(dp.speed()) << "," << dp.rateOfClimb() << "\n";
        }
    }
    return 0;
}

int summarize(const std::string& gpxFileName)
{
    Parser parser;
    auto activity = parser.parseFile(gpxFileName);
    GpxCalculator calculator;
    for (const auto& track : activity.tracks())
    {
        auto dataStream = calculator.analyzeTrack(track);
        auto summary = calculator.summarizeStream(dataStream);
        auto elapsedTime = summary.duration.count();
        std::cout << "Totalt time: " << uint64_t(elapsedTime) / 3600 << ":" << (uint64_t(elapsedTime) % 3600) / 60 << ":" << ((uint64_t(elapsedTime) % 3600) % 60) << "\n";
        std::cout << "Totalt distance: " << summary.totalDistance / 1000.0 << " km " << m_to_miles(summary.totalDistance) << " miles\n";
        std::cout << "Average speed: " << mps_to_kph(summary.totalDistance / elapsedTime) << " km/h " << mps_to_mph(summary.totalDistance / elapsedTime) << "mph\n";
        std::cout << "Total Ascent: " << summary.totalAscent << " meters " << m_to_ft(summary.totalAscent) << " feet\n";
        std::cout << "Total Descent: " << summary.totalDescent << " meters " << m_to_ft(summary.totalDescent) << " feet\n";
    }
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
