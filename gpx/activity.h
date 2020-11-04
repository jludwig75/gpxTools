#pragma once

#include "track.h"

#include <vector>

namespace gpx
{

class Activity
{
public:
    void addTrack(const Track& track);
    const std::vector<Track> tracks() const;
private:
    std::vector<Track> _tracks;
};

}