cmake_minimum_required(VERSION 3.10.1)

project(ComputeGraphTest)

add_subdirectory(ComputeGraph)

add_library(ComputeGraphTest main.cpp)

target_link_libraries(ComputeGraphTest ComputeGraph)
