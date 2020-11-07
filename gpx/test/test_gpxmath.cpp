#include "catch.hpp"

#include <cmath>

#include <cfloat>

#include "gpx/gpxmath.h"
#include "gpxtools.pb.h"

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
        gpxtools::SurfaceCoordinates a;
        a.set_latitude(90);
        a.set_longitude(0);
        gpxtools::SurfaceCoordinates b;
        b.set_latitude(90);
        b.set_longitude(1);
        REQUIRE(floats_equal(a - b, 0.0, 0.001));  // accurate to 1 mm

        gpxtools::SurfaceCoordinates x;
        x.set_latitude(-90);
        x.set_longitude(01);
        gpxtools::SurfaceCoordinates y;
        y.set_latitude(-90);
        y.set_longitude(1);
        REQUIRE(floats_equal(x - y, 0.0, 0.001));  // accurate to 1 mm
    }
    SECTION("1 Degree longitude at equator")
    {
        gpxtools::SurfaceCoordinates a;
        a.set_latitude(0);
        a.set_longitude(0);
        gpxtools::SurfaceCoordinates b;
        b.set_latitude(0);
        b.set_longitude(1);
        REQUIRE(floats_equal(a - b, 111194.927, 0.001));  // accurate to 1 mm
    }
    SECTION("1 Degree longitude at 45 degrees north")
    {
        gpxtools::SurfaceCoordinates a;
        a.set_latitude(45);
        a.set_longitude(0);
        gpxtools::SurfaceCoordinates b;
        b.set_latitude(45);
        b.set_longitude(1);
        REQUIRE(floats_equal(a - b, 78626.188, 0.001));  // accurate to 1 mm
    }
}