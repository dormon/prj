cmake_minimum_required(VERSION 3.12.1)

project(barrierTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Vars CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Barrier.h Barrier.cpp FunctionPrologue.h Timer.h costring.h)
target_link_libraries(${PROJECT_NAME} PUBLIC Vars::Vars)

add_custom_target(run ./${PROJECT_NAME})
