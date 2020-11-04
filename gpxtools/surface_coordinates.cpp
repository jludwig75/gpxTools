#include "surface_coordinates.h"


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
