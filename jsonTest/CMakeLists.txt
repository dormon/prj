cmake_minimum_required(VERSION 3.1.0)

set(APPLICATION_NAME jsonTest)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

set(_subprojects NIndex Tensor NetworkConfiguration NetworkConfigurationJSONLoader)
if("${as_subproject}" STREQUAL "")
  #IN_LIST is operator in CMake 3.3, CMake 3.8.1 warns 
  cmake_policy(SET CMP0057 NEW)
  list(APPEND as_subproject ${${LIB_NAME}_subprojects})
  macro(find_package name)
    if(NOT "${name}" IN_LIST as_subproject)
      _find_package(${ARGV})
    endif()
  endmacro()
else()
  list(APPEND as_subproject ${${LIB_NAME}_subprojects})
endif()

set(NINDEX_DYNAMIC FALSE CACHE BOOL "")
set(TENSOR_DYNAMIC FALSE CACHE BOOL "")
set(NETWORKCONFIGURATION_DYNAMIC FALSE CACHE BOOL "")
set(NETWORKCONFIGURATIONJSONLOADER_DYNAMIC FALSE CACHE BOOL "")
add_subdirectory(NIndex)
add_subdirectory(Tensor)
add_subdirectory(NetworkConfiguration)
add_subdirectory(NetworkConfigurationJSONLoader)

set(INCLUDES
  )

set(SOURCES
  main.cpp
  )

add_executable(${APPLICATION_NAME} ${INCLUDES} ${SOURCES})
target_link_libraries(${APPLICATION_NAME} 
  PUBLIC
    NIndex::NIndex
    NetworkConfiguration::NetworkConfiguration
    NetworkConfigurationJSONLoader::NetworkConfigurationJSONLoader
  )


IF(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${APPLICATION_NAME} PUBLIC -W)
  target_compile_options(${APPLICATION_NAME} PUBLIC -Wall)
  target_compile_options(${APPLICATION_NAME} PUBLIC -Wconversion)
  target_compile_options(${APPLICATION_NAME} PUBLIC -pedantic)
ENDIF()

