#include "activity.h"

namespace gpx
{

Activity::Activity()
    :
    _startTime(0)
{
}

void Activity::addTrack(Track&& track)
{
    _tracks.emplace_back(track);
}

void Activity::setStartTime(std::time_t startTime)
{
    _startTime = startTime;
}

std::time_t Activity::startTime() const
{
    return _startTime;
}

const std::vector<Track> Activity::tracks() const
{
    return _tracks;
}

}