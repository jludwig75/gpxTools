# Proto file
get_filename_component(gpx_proto "${CMAKE_CURRENT_SOURCE_DIR}/gpxtools.proto" ABSOLUTE)
get_filename_component(gpx_proto_path "${gpx_proto}" PATH)

# Generated sources
set(gpx_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/gpxtools.pb.cc")
set(gpx_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/gpxtools.pb.h")
set(gpx_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/gpxtools.grpc.pb.cc")
set(gpx_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/gpxtools.grpc.pb.h")
add_custom_command(
      OUTPUT "${gpx_proto_srcs}" "${gpx_proto_hdrs}" "${gpx_grpc_srcs}" "${gpx_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
        --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
        -I "${gpx_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${gpx_proto}"
      DEPENDS "${gpx_proto}")

add_library(gpxproto STATIC
            ${gpx_proto_srcs}
            ${gpx_grpc_srcs})

target_link_libraries(gpxproto
            ${_REFLECTION}
            ${_GRPC_GRPCPP}
            ${_PROTOBUF_LIBPROTOBUF})

set_target_properties(gpxproto PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")
  