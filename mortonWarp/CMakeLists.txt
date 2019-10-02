cmake_minimum_required(VERSION 3.12.1)

project(morton)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(glm CONFIG)

add_executable(${PROJECT_NAME} main.cpp stb_image.h)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

add_custom_target(run ./GAM)

