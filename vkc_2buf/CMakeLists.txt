cmake_minimum_required(VERSION 3.16)

project(vkc)

add_executable(${PROJECT_NAME} main.cpp main.hpp devMem.cpp devMem.hpp)
target_link_libraries(${PROJECT_NAME} vulkan)
add_custom_target(run ./${PROJECT_NAME})

option(CLEAR_CMAKE_ROOT_DIR "if this is set, #define CMAKE_ROOT_DIR will be .")

if(NOT CLEAR_CMAKE_ROOT_DIR)
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR=".")
endif()

