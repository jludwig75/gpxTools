add_executable(gpxServer
                gpxServer.cpp
                calculator.cpp
                parser.cpp
                gpxmath.cpp)

target_link_libraries(gpxServer
              gpxproto
              xerces-c)

add_subdirectory(test)
