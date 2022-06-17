cmake_minimum_required(VERSION 3.19)
project(testycm)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

find_package(ArgumentViewer CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/mylib.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(${PROJECT_NAME} ArgumentViewer::ArgumentViewer)

