
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(gpxTools VERSION 1.0)

add_subdirectory(gpx)
add_subdirectory(gpxTools)
add_subdirectory(gpxServer)
