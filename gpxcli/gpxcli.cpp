#include <iostream>
#include <string>
#include <vector>

#include "gpx/parser.h"
#include "gpx/gpxcalc.h"


int parseGpxFile(const std::string& gpxFileName)
{
    auto activity = gpx::parseFile(gpxFileName);
    std::cout << "Prased " << activity.tracks()[0].trackSegments()[0].trackPoints().size() << " track points\n";
    return 0;
}

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

int plotStats(const std::string& gpxFileName)
{
    auto activity = gpx::parseFile(gpxFileName);
    std::cout << "time,altitude (meters),grade,speed (kph),rate of climb (m/s)\n";
    gpx::GpxCalculator calculator;
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
    auto activity = gpx::parseFile(gpxFileName);
    gpx::GpxCalculator calculator;
    for (const auto& track : activity.tracks())
    {
        auto dataStream = calculator.analyzeTrack(track);
        double totalAscent = 0;
        double totalDescent = 0;
        for (const auto& dp : dataStream)
        {
            if (dp.verticalDisplacement > 0)
            {
                totalAscent += dp.verticalDisplacement;
            }
            else if (dp.verticalDisplacement < 0)
            {
                totalDescent += -dp.verticalDisplacement;
            }
        }
        auto totalDistance = dataStream.back().totalDistance;
        auto elapsedTime = dataStream.back().relStartTime;
        std::cout << "Totalt time: " << uint64_t(elapsedTime) / 3600 << ":" << (uint64_t(elapsedTime) % 3600) / 60 << ":" << ((uint64_t(elapsedTime) % 3600) % 60) << "\n";
        std::cout << "Totalt distance: " << totalDistance / 1000.0 << " km " << m_to_miles(totalDistance) << " miles\n";
        std::cout << "Average speed: " << mps_to_kph(totalDistance / elapsedTime) << " km/h " << mps_to_mph(totalDistance / elapsedTime) << "mph\n";
        std::cout << "Total Ascent: " << totalAscent << " meters " << m_to_ft(totalAscent) << " feet\n";
        std::cout << "Total Descent: " << totalDescent << " meters " << m_to_ft(totalDescent) << " feet\n";
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
