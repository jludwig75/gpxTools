#include "gpx_activity.h"


void GpxActivity::addTrack(const GpxTrack& track)
{
    _tracks.push_back(track);
}

const std::vector<GpxTrack> GpxActivity::tracks() const
{
    return _tracks;
}
