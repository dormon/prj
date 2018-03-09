cmake_minimum_required(VERSION 3.10.1)

add_subdirectory(BA)
add_subdirectory(BB)

add_library(B OBJECT)
target_sources(B INTERFACE $<TARGET_OBJECTS:BA> $<TARGET_OBJECTS:BB>)
