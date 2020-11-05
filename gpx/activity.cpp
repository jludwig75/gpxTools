#include "activity.h"

namespace gpx
{

void Activity::addTrack(Track&& track)
{
    _tracks.emplace_back(track);
}

const std::vector<Track> Activity::tracks() const
{
    return _tracks;
}

}