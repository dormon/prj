cmake_minimum_required(VERSION 3.1.0)

set(APPLICATION_NAME domain)
PROJECT(${APPLICATION_NAME})
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )


set(SOURCES
  src/main.cpp
  )

set(INCLUDES
  )

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

IF(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-W          )
  add_compile_options(-Wall       )
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic   )
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic)
ENDIF()


add_executable(${APPLICATION_NAME} ${SOURCES} ${INCLUDES})

include_directories(${${APPLICATION_NAME}_SOURCE_DIR}/include)
