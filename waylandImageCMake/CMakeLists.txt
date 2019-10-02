cmake_minimum_required(VERSION 3.12.1)

project(wayImg)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(geGL CONFIG REQUIRED)
find_package (Threads)

add_executable(${PROJECT_NAME} main.cpp stb_image.h)
target_link_libraries(${PROJECT_NAME} PUBLIC geGL::geGL -lEGL -lwayland-client -ldl -lwayland-egl ${CMAKE_THREAD_LIBS_INIT} -lm-2.29)

add_custom_target(run ./GAM)

