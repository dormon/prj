set(APPLICATION_NAME jsonTest)

set(INCLUDES
  json.hpp
  NIndex.h
  )

set(SOURCES
  main.cpp
  NIndex.cpp
  )

cmake_minimum_required(VERSION 3.1.0)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

SET(CMAKE_CXX_STANDARD 14)

IF(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic)
ENDIF()

set(PROJECT_NAME TestingProject)

PROJECT(${PROJECT_NAME})

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

set(libraryDirectory ${EXTERNAL_INSTALL_LOCATION}/lib/)

add_executable(${APPLICATION_NAME} ${INCLUDES} ${SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

