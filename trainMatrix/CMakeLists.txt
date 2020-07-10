cmake_minimum_required(VERSION 3.12.1)

project(GAM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#find_package(glm)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC)# glm)

add_custom_target(run ./GAM)
