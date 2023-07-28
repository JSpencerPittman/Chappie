cmake_minimum_required(VERSION 3.26)

project(Chappie)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include/chappie)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(Chappie demo.cpp ${SOURCES})

target_link_libraries(Chappie ${OpenCV_LIBS})