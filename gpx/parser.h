#pragma once

#include "activity.h"

#include <string>

namespace gpx
{

class Parser
{
public:
    Parser();
    ~Parser();
    Activity parseFile(const std::string& gpxFileData);
};

}   // namepsace gpx