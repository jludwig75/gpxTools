#pragma once

#include <cmath>

namespace gpx
{

template<typename Value>
Value pathag(Value a, Value b)
{
    return std::sqrt(a * a + b * b);
}

class SurfaceCoordinates;
double greatCircleDistance(const SurfaceCoordinates& pos1, const SurfaceCoordinates& pos2);

}