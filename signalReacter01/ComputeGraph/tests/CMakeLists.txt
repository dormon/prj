cmake_minimum_required(VERSION 3.10.1)

set(CMAKE_CXX_VERSION 14)

add_executable(BasicTest BasicTests.cpp catch.hpp)
target_link_libraries(BasicTest PRIVATE ComputeGraph)
