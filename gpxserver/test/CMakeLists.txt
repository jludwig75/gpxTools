add_executable(test_gpxmath
                test_gpxmath.cpp
                ../gpxmath.cpp)

target_link_libraries(test_gpxmath
                        catch2_main
                        gpxproto)

#catch_discover_tests(test_gpxmath)