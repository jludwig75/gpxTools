#pragma once

#include <cmath>

namespace gpx
{

template<typename ValueType>
ValueType pathag(ValueType a, ValueType b)
{
    return std::sqrt(a * a + b * b);
}

template<typename ValueType>
ValueType mps_to_kph(ValueType mps)
{
    return mps * 60.0 * 60.0 / 1000.0;
}

template<typename ValueType>
ValueType mps_to_mph(ValueType mps)
{
    return mps * 60.0 * 60.0 * 3.28084 / 5280.0;
}

template<typename ValueType>
ValueType m_to_miles(ValueType m)
{
    return m * 3.28084 / 5280.0;
}

template<typename ValueType>
ValueType m_to_ft(ValueType m)
{
    return m * 3.28084;
}

class SurfaceCoordinates;
double greatCircleDistance(const SurfaceCoordinates& pos1, const SurfaceCoordinates& pos2);

}   // namepsace gpx