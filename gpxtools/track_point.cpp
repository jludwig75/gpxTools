#include "track_point.h"


TrackPoint::TrackPoint(const Position& position, std::time_t time)
    :
    _position(position),
    _time(time)
{
}

const Position& TrackPoint::position() const
{
    return _position;
}

std::time_t TrackPoint::time() const
{
    return _time;
}

