#pragma once

#include "gpx_track.h"

#include <vector>


class GpxActivity
{
public:
    void addTrack(const GpxTrack& track);
    const std::vector<GpxTrack> tracks() const;
private:
    std::vector<GpxTrack> _tracks;
};
