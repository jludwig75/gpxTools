#pragma once

#include <cmath>

#include "gpxtools.pb.h"

namespace gpx
{

double operator-(const gpxtools::SurfaceCoordinates& a, const gpxtools::SurfaceCoordinates& b);

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

Displacement operator-(const gpxtools::Position& a, const gpxtools::Position& b);

}   // namepsace gpx

