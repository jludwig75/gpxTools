#pragma once

#include "track.h"

#include <ctime>
#include <vector>

namespace gpx
{

class Activity
{
public:
    Activity();
    void addTrack(Track&& track);
    void setStartTime(std::time_t startTime);

    std::time_t startTime() const;
    const std::vector<Track> tracks() const;
private:
    std::time_t _startTime;
    std::vector<Track> _tracks;
};

}