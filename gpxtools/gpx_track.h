#pragma once

#include "track_segment.h"

#include <string>

class GpxTrack
{
public:
    GpxTrack(const std::string& name, unsigned type);
    const std::string& name() const;
    unsigned type() const;
    void addTrackSegment(const TrackSegment& segment);
    std::vector<TrackSegment> trackSegments() const;
private:
    std::string _name;
    unsigned _type;
    std::vector<TrackSegment> _trackSegments;
};
