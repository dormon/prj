cmake_minimum_required(VERSION 3.1.0)

PROJECT(basic)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

set(SOURCES
  main.cpp
  )

set(INCLUDES
  include/fwd.h
  include/MyClass.h
  )

add_executable(basic ${SOURCES} ${INCLUDES})
target_include_directories(basic PUBLIC include/)

target_compile_definitions(basic PUBLIC NOF_STUDENTS=32)
