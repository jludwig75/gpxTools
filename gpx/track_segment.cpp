#include "track_segment.h"


namespace gpx
{

void TrackSegment::addTrackPoint(const TrackPoint& trackPoint)
{
    _trackPoints.push_back(trackPoint);
}


std::vector<TrackPoint> TrackSegment::trackPoints() const
{
    return _trackPoints;
}

}