#include "position.h"

#include <cmath>


template<typename Value>
Value pathag(Value a, Value b)
{
    return std::sqrt(a * a + b * b);
}

double operator-(const Position& a, const Position& b)
{
    auto surfaceDistance = b.coords() - a.coords();
    auto vertialDistance = pathag(b.altitude(), a.altitude());
    return pathag(surfaceDistance, vertialDistance);
}
