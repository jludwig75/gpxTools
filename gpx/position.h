#pragma once

#include "surface_coordinates.h"

namespace gpx
{
class Position
{
public:
    Position(SurfaceCoordinates&& coords, double altitude);
    const SurfaceCoordinates& coords() const;
    double altitude() const;
private:
    SurfaceCoordinates _coords;
    double _altitude;
};


class Displacement
{
public:
    Displacement(double horizontal, double vertical)
        :
        horizontal(horizontal),
        vertical(vertical)
    {

    }
    double horizontal;
    double vertical;
};

Displacement operator-(const Position& a, const Position& b);

}   // namepsace gpx