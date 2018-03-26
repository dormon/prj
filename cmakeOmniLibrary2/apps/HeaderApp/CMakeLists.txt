cmake_minimum_required(VERSION 3.10.1)

set(APPLICATION_NAME HeaderApp)

PROJECT(${APPLICATION_NAME})

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )


set(SOURCES
  main.cpp
  )

set(INCLUDES
  )

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

find_package(Header CONFIG REQUIRED)

#Create execitable
add_executable(${APPLICATION_NAME} ${SOURCES} ${INCLUDES} )

target_link_libraries(${APPLICATION_NAME} PUBLIC Header::Header)

#compilation flags
IF(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${APPLICATION_NAME} PRIVATE -W                )
  target_compile_options(${APPLICATION_NAME} PRIVATE -Wall             )
  target_compile_options(${APPLICATION_NAME} PRIVATE -Wconversion      )
  target_compile_options(${APPLICATION_NAME} PRIVATE -pedantic         )
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
  target_compile_options(${APPLICATION_NAME} PRIVATE -W)
  target_compile_options(${APPLICATION_NAME} PRIVATE -Wall)
  target_compile_options(${APPLICATION_NAME} PRIVATE -Wconversion)
  target_compile_options(${APPLICATION_NAME} PRIVATE -pedantic)
ENDIF()

