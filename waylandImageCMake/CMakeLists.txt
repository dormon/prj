cmake_minimum_required(VERSION 3.12.1)

project(wayImg)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(geGL CONFIG REQUIRED)
find_package(Vars CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package (Threads)

add_executable(${PROJECT_NAME} main.cpp stb_image.h util.h QuiltImage.h)

if(${CMAKE_CXX_COMPILER} STREQUAL "/usr/bin/aarch64-linux-gnu-g++")
  target_link_libraries(${PROJECT_NAME} PUBLIC geGL::geGL Vars::Vars glm -lEGL -lwayland-client -ldl -lwayland-egl ${CMAKE_THREAD_LIBS_INIT} -lm-2.29)
else()
  target_link_libraries(${PROJECT_NAME} PUBLIC geGL::geGL Vars::Vars glm -lEGL -lwayland-client -ldl -lwayland-egl ${CMAKE_THREAD_LIBS_INIT})
endif()

add_custom_target(run ./GAM)

