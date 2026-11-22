cmake_minimum_required(VERSION 4.5)

project(MazeGame LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)

option(ENABLE_GPROF "Build with gprof instrumentation (-pg)" OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

find_package(SDL3 REQUIRED CONFIG)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
PRIVATE
    src/main.c
    src/render.c
    src/window.c
    src/texture.c
    src/raycast.c
    src/game.c
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

if(ENABLE_GPROF)
    message(STATUS "gprof profiling enabled")
    target_compile_options(${PROJECT_NAME} PRIVATE -pg -fno-omit-frame-pointer)
    target_link_options(${PROJECT_NAME} PRIVATE -pg)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address;-fno-omit-frame-pointer>")
target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:-fsanitize=address>")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/lib/stb/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/)

target_link_libraries(${PROJECT_NAME} PRIVATE m SDL3::SDL3)