cmake_minimum_required(VERSION 3.13.0)

add_executable(tests TestsMain.cpp tests.cpp catch.hpp)

target_link_libraries(tests ArgumentViewer::ArgumentViewer)
