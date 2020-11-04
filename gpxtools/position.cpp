#include "position.h"

#include "gpxmath.h"

double operator-(const Position& a, const Position& b)
{
    auto surfaceDistance = b.coords() - a.coords();
    auto vertialDistance = pathag(b.altitude(), a.altitude());
    return pathag(surfaceDistance, vertialDistance);
}

Position::Position(const SurfaceCoordinates& coords, double altitude)
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
