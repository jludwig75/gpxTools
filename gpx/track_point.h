#pragma once

#include "position.h"

#include <ctime>

namespace gpx
{
class TrackPoint
{
public:
    TrackPoint(const Position& position, std::time_t time);
    const Position& position() const;
    std::time_t time() const;
private:
    Position _position;
    std::time_t _time;
};

}