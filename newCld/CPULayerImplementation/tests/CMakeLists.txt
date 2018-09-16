cmake_minimum_required(VERSION 3.10.1)

add_executable(tests TestsMain.cpp BasicTests.cpp catch.hpp)

target_link_libraries(BasicTest PRIVATE CPULayerImplementation::CPULayerImplementation)
