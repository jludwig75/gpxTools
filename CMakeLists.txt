
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(gpxTools VERSION 1.0)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

add_subdirectory(catch2)
add_subdirectory(gpx)
add_subdirectory(gpxcli)
add_subdirectory(gpxserver)
