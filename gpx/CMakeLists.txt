add_library(gpx STATIC
            activity.cpp
            gpxcalc.cpp
            parser.cpp
            track.cpp
            gpxmath.cpp
            position.cpp
            surface_coordinates.cpp
            track_point.cpp
            track_segment.cpp)

set_target_properties(gpx PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)

add_subdirectory(test)
