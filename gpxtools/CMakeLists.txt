add_library(gpxTools STATIC
            gpx_activity.cpp
            gpx_parser.cpp
            gpx_track.cpp
            gpxmath.cpp
            position.cpp
            surface_coordinates.cpp
            track_point.cpp
            track_segment.cpp)

set_target_properties(gpxTools PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)