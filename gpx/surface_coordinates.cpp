#include "surface_coordinates.h"

#include "gpxmath.h"

namespace gpx
{

SurfaceCoordinates::SurfaceCoordinates(double latitude, double longitude)
    :
    _latitude(latitude),
    _longitude(longitude)
{
}

double SurfaceCoordinates::latitude() const
{
    return _latitude;
}

double SurfaceCoordinates::longitude() const
{
    return _longitude;
}

double operator-(const SurfaceCoordinates& a, const SurfaceCoordinates& b)
{
    return greatCircleDistance(a, b);
}

}   // namepsace gpx