#include "position.h"

#include "gpxmath.h"

namespace gpx
{

Displacement operator-(const Position& a, const Position& b)
{
    auto surfaceDistance = a.coords() - b.coords();
    auto vertialDistance = a.altitude() - b.altitude();
    return Displacement(surfaceDistance, vertialDistance);
}

Position::Position(SurfaceCoordinates&& coords, double altitude)
    :
    _coords(coords),
    _altitude(altitude)
{
}

const SurfaceCoordinates& Position::coords() const
{
    return _coords;
}

double Position::altitude() const
{
    return _altitude;
}

}   // namepsace gpx