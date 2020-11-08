#include "gpxmath.h"

namespace gpx
{
namespace
{

template<typename ValueType>
ValueType radians(ValueType v)
{
    return (v * M_PI) / 180.0;
}

double greatCircleDistance(const gpxtools::SurfaceCoordinates& pos1, const gpxtools::SurfaceCoordinates& pos2)
{
    auto phi1 = radians(pos1.latitude());
    auto phi2 = radians(pos2.latitude());
    auto deltaPhi = radians(pos2.latitude() - pos1.latitude());
    auto deltaLambda = radians(pos2.longitude() - pos1.longitude());
    auto a = std::sin(deltaPhi/2) * std::sin(deltaPhi/2) + std::cos(phi1) * std::cos(phi2) * std::sin(deltaLambda / 2) * std::sin(deltaLambda / 2);
    auto c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return 6371000 * c;
}

}   // namepsace

double operator-(const gpxtools::SurfaceCoordinates& a, const gpxtools::SurfaceCoordinates& b)
{
    return gpx::greatCircleDistance(a, b);
}

Displacement operator-(const gpxtools::Position& a, const gpxtools::Position& b)
{
    auto surfaceDistance = a.coords() - b.coords();
    auto vertialDistance = a.altitude() - b.altitude();
    return Displacement(surfaceDistance, vertialDistance);
}

}   // namepsace gpx
