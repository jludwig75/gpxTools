#pragma once

#include "gpx_track.h"

#include <vector>


class GpxActivity
{
public:
    const std::vector<GpxTrack> tracks() const;
};
