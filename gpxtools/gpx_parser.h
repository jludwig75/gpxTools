#pragma once

#include "gpx_activity.h"

#include <string>

namespace gpx
{

Activity parseFile(const std::string& gpxFileName);

}