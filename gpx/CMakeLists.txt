# Proto file
get_filename_component(gpx_proto "../proto/gpxtools.proto" ABSOLUTE)
get_filename_component(gpx_proto_path "${gpx_proto}" PATH)

# Generated sources
set(gpx_proto_srcs "${CMAKE_SOURCE_DIR}/gpxserver/gpxtools.pb.cc")
set(gpx_proto_hdrs "${CMAKE_SOURCE_DIR}/gpxserver/gpxtools.pb.h")
set(gpx_grpc_srcs "${CMAKE_SOURCE_DIR}/gpxserver/gpxtools.grpc.pb.cc")
set(gpx_grpc_hdrs "${CMAKE_SOURCE_DIR}/gpxserver/gpxtools.grpc.pb.h")
add_custom_command(
      OUTPUT "${gpx_proto_srcs}" "${gpx_proto_hdrs}" "${gpx_grpc_srcs}" "${gpx_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_SOURCE_DIR}/gpxserver"
        --cpp_out "${CMAKE_SOURCE_DIR}/gpxserver"
        -I "${gpx_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${gpx_proto}"
      DEPENDS "${gpx_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_SOURCE_DIR}/gpxserver")

add_library(gpx STATIC
            ${gpx_proto_srcs}
            parser.cpp
            gpxmath.cpp)

target_link_libraries(gpx
            ${_REFLECTION}
            ${_GRPC_GRPCPP}
            ${_PROTOBUF_LIBPROTOBUF})
        
set_target_properties(gpx PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)

add_subdirectory(test)
