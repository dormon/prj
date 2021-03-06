cmake_minimum_required(VERSION 3.10.1)

#You should set up this variable
#Project name
project(SharedLibraryOnStaticLibraries LANGUAGES CXX)

#You should set up this variable
#Library name
set(LIB_NAME SharedLibraryOnStaticLibraries)

#You should set up these variables
#Library version
set(${LIB_NAME}_MAJOR_VERSION    1)
set(${LIB_NAME}_MINOR_VERSION    0)
set(${LIB_NAME}_PATH_VERSION     0)
set(${LIB_NAME}_REVISION_VERSION 0)

#You should set up this variable
#All includes
set(${LIB_NAME}_INCLUDES
  #SharedLibraryOnStaticLibraries.h
  )

#You should set up this variable
#All sources
set(${LIB_NAME}_SOURCES
  #SharedLibraryOnStaticLibraries.cpp
  )

# You should set up this variable
# list of include directories that this library require.
# this list should not contain subprojects or external libraries like OpenCL
# please note that the script will prefix each element with ${CMAKE_CURRENT_SOURCE_DIR}
# root directory and build directory of library is already in the list 
list(APPEND ${LIB_NAME}_PRIVATE_INCLUDE_DIRECTORIES   )
list(APPEND ${LIB_NAME}_PUBLIC_INCLUDE_DIRECTORIES    )
list(APPEND ${LIB_NAME}_INTERFACE_INCLUDE_DIRECTORIES )

#You should set up this variable
#list of subprojects that will be inserted into this library
list(APPEND ${LIB_NAME}_SUBPROJECTS SharedLibraryOnStaticLibraries_A SharedLibraryOnStaticLibraries_B)

set(INSTALL_INCLUDE_DIRECTORY   include/)
set(INSTALL_CONFIG_DIRECTORY    lib/cmake/${LIB_NAME})
set(GENERATED_INCLUDE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)
set(GENERATED_CONFIG_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/)

#You should set up this variable
#This variable contains external linking targets not build by this project
set(${LIB_NAME}_EXTERNAL_TARGETS )

#You should set up this variable
#This variable contains extermal libraries that needs to be find by find_package
list(APPEND ${LIB_NAME}_EXTERNAL_LIBRARIES_TO_FIND )

#You should set up this variable
#This variable contains compilation options for this library
set(${LIB_NAME}_COMPILATION_OPTION )

include(../../CMakeLibrary/libraryExports.cmake)
