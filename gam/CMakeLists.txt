cmake_minimum_required(VERSION 3.12.1)

project(GAM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(Vars CONFIG REQUIRED)
find_package(GLEW)

add_executable(${PROJECT_NAME} main.cpp createWindow.h createWindow.cpp destroyWindow.h destroyWindow.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2::SDL2main Vars::Vars GLEW GL X11 Xi)

add_custom_target(run ./GAM)
