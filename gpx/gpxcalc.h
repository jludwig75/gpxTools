#pragma once

#include <cassert>
#include <chrono>
#include <ctime>
#include <vector>


namespace gpx
{

class Track;

class DataPoint
{
public:
    DataPoint(std::time_t relStartTime, std::chrono::duration<double> duration, double totalDistance, double altitude, double horizontalDisplacement, double verticalDisplacement)
        :
        relStartTime(relStartTime),
        duration(duration),
        totalDistance(totalDistance),
        altitude(altitude),
        horizontalDisplacement(horizontalDisplacement),
        verticalDisplacement(verticalDisplacement)
    {
        assert(horizontalDisplacement >= 0);
    }
    std::time_t relStartTime;
    std::chrono::duration<double> duration;
    double altitude;
    double totalDistance;
    double horizontalDisplacement;
    double verticalDisplacement;
    double speed() const
    {
        assert(duration.count() > 0);
        return horizontalDisplacement / duration.count();
    }
    double rateOfClimb() const
    {
        assert(duration.count() > 0);
        return verticalDisplacement / duration.count();
    }
    double grade() const
    {
        if (horizontalDisplacement == 0)
        {
            return 0;
        }

        return verticalDisplacement / horizontalDisplacement;
    }
};


using DataStream = std::vector<DataPoint>;

class DataSummary
{
public:
    DataSummary(std::chrono::duration<double> duration, double totalDistance, double totalAscent, double totalDescent)
        :
        duration(duration),
        totalDistance(totalDistance),
        totalAscent(totalAscent),
        totalDescent(totalDescent)
    {
    }
    std::chrono::duration<double> duration;
    double totalDistance;
    double totalAscent;
    double totalDescent;
    double averageSpeed() const
    {
        if (duration.count() == 0)
        {
            return 0;
        }

        return totalDistance / duration.count();
    }
};

class GpxCalculator
{
public:
    DataStream analyzeTrack(const Track& track);
    DataSummary summarizeStream(const DataStream& dataStream);
};

}