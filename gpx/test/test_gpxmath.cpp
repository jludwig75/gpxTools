#include "catch.hpp"

#include <cmath>

#include <cfloat>

#include "gpx/gpxmath.h"
#include "gpx/surface_coordinates.h"


using namespace gpx;


template<typename ValueType>
ValueType floats_equal_pct(ValueType a, ValueType b,
                         ValueType maxRelDiff = FLT_EPSILON)
{
    // Calculate the difference.
    ValueType diff = std::fabs(a - b);
    a = std::fabs(a);
    b = std::fabs(b);
    // Find the largest
    ValueType largest = (b > a) ? b : a;
 
    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}

template<typename ValueType>
ValueType floats_equal(ValueType a, ValueType b, ValueType maxDiff)
{
    // Calculate the difference.
    ValueType diff = std::fabs(a - b);
    a = std::fabs(a);
    b = std::fabs(b);
    // Find the largest
    ValueType largest = (b > a) ? b : a;
 
    if (diff <= maxDiff)
        return true;
    return false;
}

TEST_CASE( "Great Circle Disance", "" )
{
    SECTION("1 Degree longitude at poles")
    {
        SurfaceCoordinates a(90, 0);
        SurfaceCoordinates b(90, 1);
        REQUIRE(floats_equal(a - b, 0.0, 0.001));  // accurate to 1 mm

        SurfaceCoordinates x(-90, 0);
        SurfaceCoordinates y(-90, 1);
        REQUIRE(floats_equal(x - y, 0.0, 0.001));  // accurate to 1 mm
    }
    SECTION("1 Degree longitude at equator")
    {
        SurfaceCoordinates a(0, 0);
        SurfaceCoordinates b(0, 1);
        REQUIRE(floats_equal(a - b, 111194.927, 0.001));  // accurate to 1 mm
    }
    SECTION("1 Degree longitude at 45 degrees north")
    {
        SurfaceCoordinates a(45, 0);
        SurfaceCoordinates b(45, 1);
        REQUIRE(floats_equal(a - b, 78626.188, 0.001));  // accurate to 1 mm
    }
}