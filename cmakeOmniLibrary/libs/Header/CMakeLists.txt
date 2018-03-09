cmake_minimum_required(VERSION 3.10.1)

#You should set up this variable
#Project name
project(Header LANGUAGES CXX)

#You should set up this variable
#Library name
set(LIB_NAME Header)

#You should set up these variables
#Library version
set(${LIB_NAME}_MAJOR_VERSION    1)
set(${LIB_NAME}_MINOR_VERSION    0)
set(${LIB_NAME}_REVISION_VERSION 0)

#You should set up this variable
#All includes
set(INCLUDES
  Header.h
  )

#You should set up this variable
#All sources
set(SOURCES
  )

#You should set up this variable
#list of subprojects
list(APPEND ${LIB_NAME}_subprojects )

#You should set up this variable
#list of subprojects that will be build as static library
list(APPEND ${LIB_NAME}_build_as_static )

set(INSTALL_INCLUDE_DIRECTORY   include/)
set(INSTALL_CONFIG_DIRECTORY    lib/cmake/${LIB_NAME})
set(GENERATED_INCLUDE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)

#You should set up this variable
#This variable contains external linking targets not build by this project
set(${LIB_NAME}_EXTERNAL_TARGETS )

#You should set up this variable
#This variable contains extermal libraries that needs to be find by find_package
list(APPEND ${LIB_NAME}_EXTERNAL_LIBRARIES_TO_FIND )

#You can set up this variable
#This variable contains compilation options (-W, -Wall, -pedantic, ...) for this library
set(${LIB_NAME}_COMPILE_OPTIONS )

#You can set up these variables
#This variable contains private compilation definition (-DUSE_RADIANSE, ...) for this library.
set(${LIB_NAME}_PRIVATE_COMPILE_OPTIONS )
set(${LIB_NAME}_PUBLIC_COMPILE_OPTIONS )
set(${LIB_NAME}_INTERFACE_COMPILE_OPTIONS )



include(../../CMakeLibrary/libraryExports.cmake)
