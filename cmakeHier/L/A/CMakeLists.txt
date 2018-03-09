cmake_minimum_required(VERSION 3.10.1)

add_library(A  OBJECT)
message("${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(A PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/A.h ${CMAKE_CURRENT_SOURCE_DIR}/A.cpp)

