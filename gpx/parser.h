#pragma once

#include "activity.h"

#include <string>

namespace gpx
{

Activity parseFile(const std::string& gpxFileName);

}   // namepsace gpx