add_executable(gpxServer
                gpxServer.cpp
                calculator.cpp
                parser.cpp)

target_link_libraries(gpxServer
              gpx
              gpxproto)
