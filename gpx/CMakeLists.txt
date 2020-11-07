add_library(gpx STATIC
            parser.cpp
            gpxmath.cpp)

target_link_libraries(gpx
            gpxproto
            xerces-c)

set_target_properties(gpx PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}"
)

add_subdirectory(test)
