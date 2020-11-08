add_executable(gpxcli
                gpxcli.cpp
                gpxcalculator.cpp
                gpxparser.cpp)

target_link_libraries(gpxcli
                      gpxproto)
