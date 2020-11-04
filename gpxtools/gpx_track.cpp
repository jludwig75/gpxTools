#include "gpx_track.h"


GpxTrack::GpxTrack(const std::string& name, unsigned type)
    :
    _name(name),
    _type(type)
{
}

const std::string& GpxTrack::name() const
{
    return _name;
}

unsigned GpxTrack::type() const
{
    return _type;
}

void GpxTrack::addTrackSegment(const TrackSegment& segment)
{
    _trackSegments.push_back(segment);
}

std::vector<TrackSegment> GpxTrack::trackSegments() const
{
    return _trackSegments;
}
