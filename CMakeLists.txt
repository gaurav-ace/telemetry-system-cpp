cmake_minimum_required(VERSION 3.15)
project(telemetry_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(async_logger
	logger/AsyncLogger.cpp
)

target_include_directories(async_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(logger_demo_app
    applications/logger_demo_app.cpp
)

target_link_libraries(logger_demo_app async_logger pthread)

