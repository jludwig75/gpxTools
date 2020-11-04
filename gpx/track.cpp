#include "track.h"

namespace gpx
{

Track::Track(const std::string& name, unsigned type)
    :
    _name(name),
    _type(type)
{
}

const std::string& Track::name() const
{
    return _name;
}

unsigned Track::type() const
{
    return _type;
}

void Track::addTrackSegment(const TrackSegment& segment)
{
    _trackSegments.push_back(segment);
}

std::vector<TrackSegment> Track::trackSegments() const
{
    return _trackSegments;
}

}