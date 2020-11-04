#pragma once


class SurfaceCoordinates
{
public:
    double latitude() const;
    double longitude() const;
};

double operator-(const SurfaceCoordinates& a, const SurfaceCoordinates& b);