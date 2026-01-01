cmake_minimum_required(VERSION 3.15)
project(telemetry_system)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(PROTO_FILE ${PROTO_DIR}/telemetry.proto)


set(GENERATED_PROTO_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/telemetry.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/telemetry.grpc.pb.cc
)

set(GENERATED_PROTO_HDR
    ${CMAKE_CURRENT_BINARY_DIR}/telemetry.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/telemetry.grpc.pb.h
)

# Protobuf + gRPC code generation
add_custom_command(
    OUTPUT ${GENERATED_PROTO_SRC} ${GENERATED_PROTO_HDR}
    COMMAND protobuf::protoc
    ARGS
        --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
        --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        -I ${PROTO_DIR}
        ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating protobuf and gRPC sources"
)

#------------------------------
# async_logger library
# -----------------------------
add_library(async_logger
	logger/AsyncLogger.cpp
)

target_include_directories(async_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


#---------------------------
# telemetry library
#---------------------------

file(GLOB SOURCES "telemetry/*.cpp")

add_library(telemetry
	${SOURCES}
)


target_include_directories(async_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


target_include_directories(telemetry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(telemetry
    telemetry_proto
)

## logger demo executable
# --------------------------
add_executable(logger_demo_app
    applications/logger_demo_app.cpp
)

target_link_libraries(logger_demo_app async_logger pthread)


## metrics demo executable
# --------------------------
add_executable(metrics_demo_app
    applications/metrics_demo_app.cpp
)

target_link_libraries(metrics_demo_app
    telemetry
    pthread
)

## telemetry demo executable
#---------------------------
add_executable(telemetry_demo_app
	applications/telemetry_demo_app.cpp
)

target_link_libraries(telemetry_demo_app
    telemetry
    async_logger
    telemetry_proto
    pthread
)

# ----------------------------
# proto library
# ---------------------------
add_library(telemetry_proto
	${GENERATED_PROTO_SRC}
)

target_include_directories(telemetry_proto PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(telemetry_proto
    protobuf::libprotobuf
    gRPC::grpc++
)

#-------------------
# telemetry server
#-------------------
add_executable(telemetry_server
	server/telemetry_server.cpp
)

target_link_libraries(telemetry_server
    telemetry_proto
    gRPC::grpc++
    protobuf::libprotobuf
)

