#pragma once

#include "track_point.h"

#include <vector>

namespace gpx
{
class TrackSegment
{
public:
    void addTrackPoint(const TrackPoint& trackPoint);
    std::vector<TrackPoint> trackPoints() const;
private:
    std::vector<TrackPoint> _trackPoints;
};

}