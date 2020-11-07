add_executable(gpxServer
                gpxServer.cpp)

target_link_libraries(gpxServer
              gpx
              gpxproto)
