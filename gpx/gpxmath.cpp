#include "gpxmath.h"

#include "surface_coordinates.h"

namespace gpx
{
namespace
{

template<typename Value>
Value radians(Value v)
{
    (v * M_PI) / 180.0;
}

}   // namepsace

double greatCircleDistance(const SurfaceCoordinates& pos1, const SurfaceCoordinates& pos2)
{
    auto phi1 = radians(pos1.latitude());
    auto phi2 = radians(pos2.latitude());
    auto deltaPhi = radians(pos2.latitude() - pos1.latitude());
    auto deltaLambda = radians(pos2.longitude() - pos1.longitude());
    auto a = std::sin(deltaPhi/2) * std::sin(deltaPhi/2) + std::cos(phi1) * std::cos(phi2) * std::sin(deltaLambda / 2) * std::sin(deltaLambda / 2);
    auto c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return 6371000 * c;
}
   
}   // namepsace gpx