cmake_minimum_required(VERSION 3.16)

project(vkc)

SET(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/main.hpp
  src/devMem.cpp
  src/devMem.hpp
  src/createBuffer.cpp
  src/createBuffer.hpp
  src/createShaderModule.cpp
  src/createShaderModule.hpp
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

message("${CMAKE_BINARY_DIR}")

set(COMPILED_SHADER_DIR ${CMAKE_BINARY_DIR}/shaders)

# create directory inside build for built shaders
file(MAKE_DIRECTORY ${COMPILED_SHADER_DIR})

# create command for building shader
add_custom_target(
  compileShaders
  ALL
  Vulkan::glslangValidator -S comp resources/shader.glsl -V -o ${COMPILED_SHADER_DIR}/shader.spv
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS           resources/shader.glsl
  BYPRODUCTS        ${COMPILED_SHADER_DIR}/shader.spv
  )

add_dependencies(run compileShaders)

# this property sets up visual studio start-up project to the project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
