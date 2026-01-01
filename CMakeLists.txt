cmake_minimum_required(VERSION 3.15)
project(telemetry_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

target_include_directories(telemetry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



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
    pthread
)


