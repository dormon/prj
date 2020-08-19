cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

project(remsub)

option(USE_OPENCV "use opencv" ON)

if(USE_OPENCV)
  find_package(OpenCV REQUIRED)
  set(VIDEO_SOURCES
    video.hpp
    )
  set(VIDEO_INCLUDES
    ${OpenCV_INCLUDE_DIRS} 
    )
  set(VIDEO_LIBS
    ${OpenCV_LIBS} 
    )
  set(VIDEO_DEFINES
    USE_OPENCV

    )
else()
  find_package(FFMPEG REQUIRED)
  set(VIDEO_SOURCES 
    CachedVideo.hpp
    CachedVideo.cpp
    )
  set(VIDEO_INCLUDES
    ${FFMPEG_INCLUDE_DIRS}
    ${SWSCALE_INCLUDE_DIRS}
    )
  set(VIDEO_LIBS
    ${FFMPEG_LIBRARIES}
    ${SWSCALE_LIBRARIES}
    )
  set(VIDEO_DEFINES

    )
endif()

find_package(glm            CONFIG REQUIRED)
find_package(ArgumentViewer CONFIG REQUIRED)
find_package(Simple3DApp    CONFIG REQUIRED)
find_package(imguiVars      CONFIG REQUIRED)
find_package(VarsGLMDecorator CONFIG REQUIRED)



add_executable(${PROJECT_NAME} 
  main.cpp 
  Barrier.cpp 
  Barrier.h 
  FunctionPrologue.h
  keyframes.hpp
  json.hpp
  stb_image_write.h
  loadTxtFile.hpp
  loadTxtFile.cpp
  saveTxtFile.hpp
  saveTxtFile.cpp
  createDrawProgram.hpp
  createDrawProgram.cpp
  createComputeProgram.hpp
  createComputeProgram.cpp
  drawFinalFrame.hpp
  drawFinalFrame.cpp
  Project.hpp
  Project.cpp
  VideoManager.hpp
  VideoManager.cpp
  Stream.hpp
  Stream.cpp
  getTimeFormat.hpp
  getTimeFormat.cpp
  computeFrame.hpp
  computeFrame.cpp
  editProgram.hpp
  editProgram.cpp
  saveFragment.hpp
  saveFragment.cpp
  loadFragment.hpp
  loadFragment.cpp
  ${VIDEO_SOURCES}
  )
target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${VIDEO_INCLUDES}
  )
target_link_libraries(${PROJECT_NAME} PUBLIC 
  glm 
  Simple3DApp::Simple3DApp 
  geGL::geGL 
  ArgumentViewer::ArgumentViewer 
  VarsGLMDecorator::VarsGLMDecorator 
  imguiVars::imguiVars
  ${VIDEO_LIBS}
  )
target_compile_definitions(${PROJECT_NAME} PUBLIC ${VIDEO_DEFINES})
