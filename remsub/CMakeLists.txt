cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

find_package(OpenCV                REQUIRED)
find_package(glm            CONFIG REQUIRED)
find_package(ArgumentViewer CONFIG REQUIRED)
find_package(Simple3DApp    CONFIG REQUIRED)
find_package(imguiVars      CONFIG REQUIRED)
find_package(VarsGLMDecorator CONFIG REQUIRED)

project(remsub)

add_executable(${PROJECT_NAME} 
  main.cpp 
  Barrier.cpp 
  Barrier.h 
  FunctionPrologue.h
  video.hpp
  )
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} glm Simple3DApp::Simple3DApp geGL::geGL ArgumentViewer::ArgumentViewer VarsGLMDecorator::VarsGLMDecorator imguiVars::imguiVars)
