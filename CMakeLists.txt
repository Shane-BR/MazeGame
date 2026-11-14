cmake_minimum_required(VERSION 4.5)

project(MazeGame LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

find_package(SDL3 REQUIRED CONFIG)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
PRIVATE
    src/main.c
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)