add_library(gpx STATIC
            gpxmath.cpp)

target_link_libraries(gpx
            gpxproto)

set_target_properties(gpx PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_subdirectory(test)
