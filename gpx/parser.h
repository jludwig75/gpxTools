#pragma once

#include "gpxtools.pb.h"

#include <string>

namespace gpx
{

class Parser
{
public:
    Parser();
    ~Parser();
    bool parseFile(const std::string& gpxFileData, gpxtools::Activity* activity);
};

}   // namepsace gpx