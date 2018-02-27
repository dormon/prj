cmake_minimum_required(VERSION 3.10.1)

project(ComposedLibrary)

add_library(A  OBJECT L/A/A.h     L/A/A.cpp    )
add_library(BA OBJECT L/B/BA/BA.h L/B/BA/BA.cpp)
add_library(BB OBJECT L/B/BB/BB.h L/B/BB/BB.cpp)

add_library(B OBJECT)
target_sources(B INTERFACE $<TARGET_OBJECTS:BA> $<TARGET_OBJECTS:BB>)

add_library(CA OBJECT L/C/CA/CA.h L/C/CA/CA.cpp)
add_library(C  OBJECT L/C/C.h     L/C/C.cpp    )
target_sources(C INTERFACE $<TARGET_OBJECTS:CA>)

add_library(L)
target_sources(L PUBLIC 
  $<TARGET_PROPERTY:A,INTERFACE_SOURCES> $<TARGET_OBJECTS:A> 
  $<TARGET_PROPERTY:B,INTERFACE_SOURCES> $<TARGET_OBJECTS:B>
  $<TARGET_PROPERTY:C,INTERFACE_SOURCES> $<TARGET_OBJECTS:C>)

#include(print_target_properties.cmake)
#print_target_properties(L)
