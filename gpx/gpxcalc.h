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
    DataPoint(std::time_t startTime, std::chrono::duration<double> duration, double altitude, double horizontalDisplacement, double verticalDisplacement)
        :
        startTime(startTime),
        duration(duration),
        altitude(altitude),
        horizontalDisplacement(horizontalDisplacement),
        verticalDisplacement(verticalDisplacement)
    {
        assert(horizontalDisplacement >= 0);
    }
    std::time_t startTime;
    std::chrono::duration<double> duration;
    double altitude;
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

class GpxCalculator
{
public:
    DataStream analyzeTrack(const Track& track);
};

}