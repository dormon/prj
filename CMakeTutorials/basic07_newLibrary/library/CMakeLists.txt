cmake_minimum_required(VERSION 3.1.0)

PROJECT(basic)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

set(SOURCES
  Timer.cpp
  )

set(INCLUDES
  Timer.h
  )

add_library(Timer ${SOURCES} ${INCLUDES})
add_library(Timer::Timer ALIAS Timer)

#vygenerujeme timer_export.h
include(GenerateExportHeader)
generate_export_header(Timer)


target_include_directories(Timer PUBLIC
  #slozky s includes pro build
  $<BUILD_INTERFACE:${basic_SOURCE_DIR}/>  
  $<BUILD_INTERFACE:${basic_BINARY_DIR}/>  
  #slozky s includes pro install
  $<INSTALL_INTERFACE:include/> 
)

install(TARGETS Timer EXPORT TimerTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
  )


install(FILES
  ${INCLUDES}
  DESTINATION include/
  )

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/timer_export.h"
  DESTINATION
  include/
  COMPONENT
  Devel
  )


include(CMakePackageConfigHelpers)

export(EXPORT TimerTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/TimerTargets.cmake"
  NAMESPACE Timer::
  )
configure_file(cmake/TimerConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/TimerConfig.cmake"
  COPYONLY
  )

set(ConfigPackageLocation lib/cmake/Timer)
install(EXPORT TimerTargets
  FILE
  TimerTargets.cmake
  NAMESPACE
  Timer::
  DESTINATION
  ${ConfigPackageLocation}
  )
install(
  FILES
  cmake/TimerConfig.cmake
  DESTINATION
  ${ConfigPackageLocation}
  COMPONENT
  Devel
  )

