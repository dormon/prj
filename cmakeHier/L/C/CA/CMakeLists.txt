cmake_minimum_required(VERSION 3.10.1)

add_library(CA OBJECT)
target_sources(CA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CA.h ${CMAKE_CURRENT_SOURCE_DIR}/CA.cpp)

