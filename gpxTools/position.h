#pragma once

#include "surface_coordinates.h"


class Position
{
public:
    const SurfaceCoordinates& coords() const;
    double altitude() const;
};

double operator-(const Position& a, const Position& b);