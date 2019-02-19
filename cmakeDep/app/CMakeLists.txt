cmake_minimum_required(VERSION 3.13)

find_package(libb 1.0 CONFIG REQUIRED)

add_executable(dormon main.cpp)
target_link_libraries(dormon libb::libb)
