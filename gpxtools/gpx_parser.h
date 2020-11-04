#pragma once

#include "gpx_activity.h"

#include <string>


class GpxParser
{
public:
    GpxActivity parseFile(const std::string& gpxFileName) const;
};
