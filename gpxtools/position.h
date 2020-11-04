#pragma once

#include "surface_coordinates.h"

namespace gpx
{
class Position
{
public:
    Position(const SurfaceCoordinates& coords, double altitude);
    const SurfaceCoordinates& coords() const;
    double altitude() const;
private:
    SurfaceCoordinates _coords;
    double _altitude;
};

double operator-(const Position& a, const Position& b);

}