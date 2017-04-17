cmake_minimum_required(VERSION 3.5)
project(ParallelPractice_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.cpp Task1.h Task1.cpp Task2.h Task2.cpp)
add_executable(ParallelPractice_2 ${SOURCE_FILES})