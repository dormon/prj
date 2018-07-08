cmake_minimum_required(VERSION 3.10.1)

project(imguiExample)

SET(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(REVISION_VERSION 0)

#include(GenerateExportHeader)

find_package(imguiOpenGLDormon CONFIG REQUIRED)
find_package(imguiSDL2Dormon CONFIG REQUIRED)
find_package(SDL2CPP CONFIG REQUIRED)

set(SOURCES
  main.cpp
  )

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(${PROJECT_NAME} PUBLIC imguiOpenGLDormon::imguiOpenGLDormon imguiSDL2Dormon::imguiSDL2Dormon SDL2CPP::SDL2CPP)

