cmake_minimum_required(VERSION 3.16)

project(vkc)

SET(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/main.hpp
  src/devMem.cpp
  src/devMem.hpp
  src/error.cpp
  src/error.hpp
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

function(add_shader type name)
  add_custom_command(
    OUTPUT            ${COMPILED_SHADER_DIR}/${name}.spv
    COMMAND           Vulkan::glslangValidator -S ${type} resources/${name}.glsl -V -o ${COMPILED_SHADER_DIR}/${name}.spv
    DEPENDS           resources/${name}.glsl
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  add_custom_target(shader_${name} ALL
     DEPENDS ${COMPILED_SHADER_DIR}/${name}.spv
  )
endfunction()

add_shader(vert vs)
add_shader(frag fs)
add_shader(comp shader)

add_dependencies(run shader_shader shader_vs shader_fs)

# this property sets up visual studio start-up project to the project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
