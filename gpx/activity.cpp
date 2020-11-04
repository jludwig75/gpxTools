#include "activity.h"

namespace gpx
{

void Activity::addTrack(const Track& track)
{
    _tracks.push_back(track);
}

const std::vector<Track> Activity::tracks() const
{
    return _tracks;
}

}