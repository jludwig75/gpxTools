#include "track_segment.h"


namespace gpx
{

void TrackSegment::addTrackPoint(TrackPoint&& trackPoint)
{
    _trackPoints.emplace_back(trackPoint);
}


std::vector<TrackPoint> TrackSegment::trackPoints() const
{
    return _trackPoints;
}

}   // namepsace gpx