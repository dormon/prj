cmake_minimum_required(VERSION 3.1.0)

PROJECT(basic)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

set(SOURCES
  main.cpp
  )

set(INCLUDES
  )

find_package(Timer CONFIG)

add_executable(basic ${SOURCES} ${INCLUDES})
target_link_libraries(basic Timer::Timer)

