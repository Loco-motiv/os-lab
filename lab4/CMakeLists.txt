cmake_minimum_required(VERSION 3.10)
project(lab4)

include_directories(${CMAKE_SOURCE_DIR})
add_library(lib1 SHARED lib1.cpp)
add_library(lib2 SHARED lib2.cpp)
add_executable(linking main_link.cpp)
add_executable(loading main_load.cpp)
target_link_libraries(linking lib1)
target_link_libraries(loading lib1 lib2)