cmake_minimum_required(VERSION 3.10.1)

add_subdirectory(CA)

add_library(C  OBJECT)
target_sources(C  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/C.h ${CMAKE_CURRENT_SOURCE_DIR}/C.cpp)
target_sources(C INTERFACE $<TARGET_OBJECTS:CA>)
