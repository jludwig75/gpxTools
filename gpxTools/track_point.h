#pragma once

#include "position.h"

#include <ctime>

class TrackPoint
{
public:
    const Position& position() const;
    std::time_t time() const;
};

