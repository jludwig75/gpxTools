#pragma once

#include "track_segment.h"

#include <string>

class GpxTrack
{
public:
    const std::string& name() const;
    unsigned type() const;
    std::vector<TrackSegment> trackSegments() const;
};
