cmake_minimum_required(VERSION 3.5)
project(TasksOpenMP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES 1.h 2.h main.cpp 3.h Tree.h)
add_executable(TasksOpenMP ${SOURCE_FILES} main.cpp)