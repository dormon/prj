cmake_minimum_required(VERSION 3.16)

project(vkc)

SET(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/main.hpp
  )

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

option(CLEAR_ROOT_DIR "if this is set, #define ROOT_DIR will be .")

if(NOT CLEAR_ROOT_DIR)
  target_compile_definitions(${PROJECT_NAME} PUBLIC ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC ROOT_DIR=".")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src/)

add_custom_target(run ./${PROJECT_NAME})

# this property sets up visual studio start-up project to the project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
