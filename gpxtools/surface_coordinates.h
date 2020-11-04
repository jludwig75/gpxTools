#pragma once

namespace gpx
{

class SurfaceCoordinates
{
public:
    SurfaceCoordinates(double latitude, double longitude);
    double latitude() const;
    double longitude() const;
private:
    double _latitude;
    double _longitude;    
};

double operator-(const SurfaceCoordinates& a, const SurfaceCoordinates& b);

}