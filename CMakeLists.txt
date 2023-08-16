cmake_minimum_required(VERSION 3.26)

project(Chappie)

set(CMAKE_CXX_STANDARD 17)

# Set the build type to Debug by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Possible build types
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Debug build flags
set(CMAKE_CXX_FILES_DEBUG "${CMAKE_CXX_FILES_DEBUG} -Wall -g")

# Release build flags
set(CMAKE_CXX_FILES_RELEASE "${CMAKE_CXX_FILES_RELEASE} -O3")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include/chappie)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(Chappie demo.cpp ${SOURCES})

target_link_libraries(Chappie ${OpenCV_LIBS})