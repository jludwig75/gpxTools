#pragma once

#include "track_point.h"

#include <vector>

class TrackSegment
{
public:
    std::vector<TrackPoint> trackPoints() const;
};

