cmake_minimum_required(VERSION 3.26)

project(ChappieTest)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Catch2 3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(../include/chappie)

#file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(test tests.cpp)

#target_link_libraries(Chappie ${OpenCV_LIBS} Catch2)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)