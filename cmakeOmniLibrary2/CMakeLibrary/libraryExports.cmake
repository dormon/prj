# ISSUES:
#
# - add compile definitions
# - add compile flags
######## - target_include_directories does not work as expected
######## - newer version of CMake
# - creating shared library from 2 or more static subproject libraries (dll/so does not contain all symbols)
# - creating static library from 2 or more static subproject libraries
# - can we generate ${LIB_NAME}Config.cmake? - yes we can
# - rename DYNAMIC option to SHARED option
# - _find_package breaks CMake (test it on newer cmakes)
# - support external libraries that do not provide Configs
# - redesign namespaces
# - add documentation
# - add "build_as_static" variable containing all subprojects that you want to build as static
#

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

function(msg)
  list(GET ARGV 0 MessageType)
  if(MessageType STREQUAL FATAL_ERROR OR MessageType STREQUAL SEND_ERROR)
    list(REMOVE_AT ARGV 0)
    message(${MessageType} "${BoldRed}${ARGV}${ColourReset}")
  elseif(MessageType STREQUAL WARNING)
    list(REMOVE_AT ARGV 0)
    message(${MessageType} "${BoldYellow}${ARGV}${ColourReset}")
  elseif(MessageType STREQUAL AUTHOR_WARNING)
    list(REMOVE_AT ARGV 0)
    message(${MessageType} "${BoldCyan}${ARGV}${ColourReset}")
  elseif(MessageType STREQUAL STATUS)
    list(REMOVE_AT ARGV 0)
    message(${MessageType} "${Green}${ARGV}${ColourReset}")
  else()
    message("${ARGV}")
  endif()
endfunction()


#This should be specified by user of this file
#SET(CMAKE_CXX_STANDARD 14)
#SET(CMAKE_C_STANDARD   99)

set(LIB_NAME_UPPER)
set(LIB_NAME_LOWER)
string(TOUPPER ${LIB_NAME} LIB_NAME_UPPER)
string(TOLOWER ${LIB_NAME} LIB_NAME_LOWER)

# The first step: we need to determine the type of this library
# (I AM     SUBPROJECT) + (I HAVE NO SOURCES) + (I DONT HAVE SUBPROJECTS  ) -> INTERFACE     | HEADER ONLY
# (I AM     SUBPROJECT) + (I HAVE NO SOURCES) + (I HAVE HEADER SUBPROJECTS) -> INTERFACE     | HEADER ONLY
# (I AM     SUBPROJECT) + (I HAVE NO SOURCES) + (I HAVE OBJECT SUBPROJECTS) -> OBJECT        |
# (I AM     SUBPROJECT) + (I HAVE    SOURCES) + (I DONT HAVE SUBPROJECTS  ) -> OBJECT        |
# (I AM     SUBPROJECT) + (I HAVE    SOURCES) + (I HAVE HEADER SUBPROJECTS) -> OBJECT        |
# (I AM     SUBPROJECT) + (I HAVE    SOURCES) + (I HAVE OBJECT SUBPROJECTS) -> OBJECT        |
# (I AM NOT SUBPROJECT) + (I HAVE NO SOURCES) + (I DONT HAVE SUBPROJECTS  ) -> INTERFACE     | HEADER ONLY
# (I AM NOT SUBPROJECT) + (I HAVE NO SOURCES) + (I HAVE HEADER SUBPROJECTS) -> INTERFACE     | HEADER ONLY
# (I AM NOT SUBPROJECT) + (I HAVE NO SOURCES) + (I HAVE OBJECT SUBPROJECTS) -> SHARED/STATIC |
# (I AM NOT SUBPROJECT) + (I HAVE    SOURCES) + (I DONT HAVE SUBPROJECTS  ) -> SHARED/STATIC |
# (I AM NOT SUBPROJECT) + (I HAVE    SOURCES) + (I HAVE HEADER SUBPROJECTS) -> SHARED/STATIC |
# (I AM NOT SUBPROJECT) + (I HAVE    SOURCES) + (I HAVE OBJECT SUBPROJECTS) -> SHARED/STATIC |


# TODO check if subproject is header only
if("${SOURCES}" STREQUAL "" AND "${${LIB_NAME}_SUBPROJECTS}" STREQUAL "")
  set(HEADER_ONLY_LIBRARY ON)
else()
  set(HEADER_ONLY_LIBRARY OFF)
endif()

# The 2. step: we need to determine type of this library.
# 1. If It is header only library the type is INTERFACE
# 2. If It is not header only library we should let user choose between:
# STATIC and SHARED
# 3. If This library is component of another library and this library is not header library:
# this library should be object library

OPTION(${LIB_NAME}_SUBPROJECT "Build object library (*.obj,*.o,...). Use OFF for any other lib." OFF)

# TODO WE ARE NOT CHECKING FOR HEADER ONLY SUBPROJECTS
if(${LIB_NAME}_SUBPROJECT)
  # this library is subproject
  if((NOT "${${LIB_NAME}_SUBPROJECTS}" STREQUAL "") OR (NOT "${SOURCES}" STREQUAL ""))
    set(${LIB_NAME}_LIBRARY_TYPE "OBJECT")
  else()
    set(${LIB_NAME}_LIBRARY_TYPE "INTERFACE")
  endif()
else()
  # this library is not subproject
  if((NOT "${${LIB_NAME}_SUBPROJECTS}" STREQUAL "") OR (NOT "${SOURCES}" STREQUAL ""))
    OPTION(${LIB_NAME}_SHARED "Build dynamic library (dll,so,...). Use OFF for static lib." ON)
    if(${LIB_NAME}_SHARED)
      set(${LIB_NAME}_LIBRARY_TYPE "SHARED")
    else()
      set(${LIB_NAME}_LIBRARY_TYPE "STATIC")
    endif()
  else()
    set(${LIB_NAME}_LIBRARY_TYPE "INTERFACE")
  endif()
endif()

message("${LIB_NAME}_SUBPROJECT is: ${${LIB_NAME}_SUBPROJECT}")
message("${LIB_NAME}_LIBRARY_TYPE is: ${${LIB_NAME}_LIBRARY_TYPE}")
message("${LIB_NAME}_SUBPROJECTS is: ${${LIB_NAME}_SUBPROJECTS}")

# create this library and create alias
add_library(${LIB_NAME} ${${LIB_NAME}_LIBRARY_TYPE})
add_library(${LIB_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})

# set every subproject to be built as object
foreach(subdir ${${LIB_NAME}_SUBPROJECTS})
  set(${subdir}_SUBPROJECT TRUE CACHE BOOL "")
  add_subdirectory(${subdir})
endforeach()

# add subproject sources and objects (*.o)
foreach(subdir ${${LIB_NAME}_SUBPROJECTS})
  target_sources(${LIB_NAME} PUBLIC $<TARGET_PROPERTY:${subdir},INTERFACE_SOURCES>)
  if(${LIB_NAME}_SUBPROJECT)
    target_sources(${LIB_NAME} INTERFACE $<TARGET_OBJECTS:${subdir}>)
  else()
    target_sources(${LIB_NAME} PUBLIC    $<TARGET_OBJECTS:${subdir}>)
  endif()
endforeach()


# add all sources and includes
if(${${LIB_NAME}_LIBRARY_TYPE} STREQUAL "INTERFACE")
  target_sources(${LIB_NAME} INTERFACE $<BUILD_INTERFACE:${INCLUDES}> $<BUILD_INTERFACE:${SOURCES}>)
else()
  foreach(inc ${INCLUDES})
    target_sources(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${inc})
  endforeach()
  foreach(src ${SOURCES})
    target_sources(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${src})
  endforeach()
endif()

if(${${LIB_NAME}_LIBRARY_TYPE} STREQUAL "INTERFACE")
  set(INTERFACE_TYPE INTERFACE)
else()
  set(INTERFACE_TYPE PUBLIC)
endif()


#########################################
#set include directories for this library
#########################################

# source root
target_include_directories(${LIB_NAME} ${INTERFACE_TYPE} $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# build directory root
target_include_directories(${LIB_NAME} ${INTERFACE_TYPE} $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

# all user specified private directories
foreach(incdir ${${LIB_NAME}_PRIVATE_INCLUDE_DIRECTORIES})
  target_include_directories(${LIB_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${incdir}>)
endforeach()

# all user specified public directories
foreach(incdir ${${LIB_NAME}_PUBLIC_INCLUDE_DIRECTORIES})
  target_include_directories(${LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${incdir}>)
endforeach()

# all user specified interface directories
foreach(incdir ${${LIB_NAME}_INTERFACE_INCLUDE_DIRECTORIES})
  target_include_directories(${LIB_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${incdir}>)
endforeach()

# TODO add include directories of 3rd party libraries


######################
# set link libraries #
######################

#all subdirectories are linked to this library as private
#TODO we should specify PRIVATE, PUBLIC, INTERFACE relation
#foreach(subdir ${${LIB_NAME}_SUBPROJECTS})
#  target_link_libraries(${LIB_NAME} INTERFACE ${subdir}::${subdir})
#endforeach()

# TODO find libraries
# TODO link to 3rd party libraries


#generate compile commands json file (usefull for vim pluggin YCM)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#sets postfixes for built libraries
SET(CMAKE_DEBUG_POSTFIX          "d"  CACHE STRING "add a postfix, usually d on windows"    )
SET(CMAKE_RELEASE_POSTFIX        ""   CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX     "s"  CACHE STRING "add a postfix, usually empty on windows")

#construct library version from version components
set(${LIB_NAME}_VERSION ${${LIB_NAME}_MAJOR_VERSION}.${${LIB_NAME}_MINOR_VERSION}.${${LIB_NAME}_REVISION_VERSION})

#set library version
#set *.so version
#set library major version
#set version compatibility
IF(NOT ${${LIB_NAME}_LIBRARY_TYPE} STREQUAL "INTERFACE")
  set_property(TARGET ${LIB_NAME}        PROPERTY VERSION                             ${${LIB_NAME}_VERSION}      )
  set_property(TARGET ${LIB_NAME}        PROPERTY SOVERSION                           ${${LIB_NAME}_MAJOR_VERSION})
ENDIF()
set_property(TARGET ${LIB_NAME}        PROPERTY INTERFACE_${LIB_NAME}_MAJOR_VERSION ${${LIB_NAME}_MAJOR_VERSION})
set_property(TARGET ${LIB_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING         ${LIB_NAME}_MAJOR_VERSION   )

# generate _export.h file and install it
if(NOT ${HEADER_ONLY_LIBRARY})
  include(GenerateExportHeader)
  generate_export_header(${LIB_NAME} EXPORT_FILE_NAME ${GENERATED_INCLUDE_DIRECTORY}/${LIB_NAME_LOWER}_export.h)
  install(FILES ${GENERATED_INCLUDE_DIRECTORY}/${LIB_NAME_LOWER}_export.h DESTINATION ${INSTALL_INCLUDE_DIRECTORY} COMPONENT Devel)
endif()

#install includes
install(FILES
  ${INCLUDES}
  DESTINATION ${INSTALL_INCLUDE_DIRECTORY}
  )
 

if(NOT ${LIB_NAME}_SUBPROJECT)
  install(
    TARGETS  ${LIB_NAME} EXPORT ${LIB_NAME}Targets
    LIBRARY  DESTINATION lib
    ARCHIVE  DESTINATION lib
    RUNTIME  DESTINATION bin
    OBJECTS  DESTINATION obj
    INCLUDES DESTINATION include
    )
  
  #generate Version config
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
    VERSION       ${${LIB_NAME}_VERSION}
    COMPATIBILITY SameMajorVersion
    )
  
  # generate *Config.cmake file
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
    "include(\${CMAKE_CURRENT_LIST_DIR}/${LIB_NAME}Targets.cmake)"
    )
  
  export(
    EXPORT    ${LIB_NAME}Targets
    FILE      ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Targets.cmake
    NAMESPACE ${LIB_NAME}::
    )
  
  install(
    EXPORT      ${LIB_NAME}Targets
    DESTINATION ${INSTALL_CONFIG_DIRECTORY}
    NAMESPACE   ${LIB_NAME}::
    FILE        ${LIB_NAME}Targets.cmake
    )
  
  #install Config and ConfigVersion files
  install(
    FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
    DESTINATION
      ${INSTALL_CONFIG_DIRECTORY}
    COMPONENT
      Devel
    )

endif()








#function(isLibraryHeaderOnly returnValue libraryName)
#  if(NOT ${${libraryName}_SOURCES} STREQUAL "")
#    set(${returnValue} false PARENT_SCOPE)
#    return()
#  endif()
#  foreach(subLibrary ${${LIB_NAME}_SUBPROJECTS})
#    isLibraryHeaderOnly(subLibraryIsHeaderOnly ${subLibrary})
#    if(NOT ${subLibraryIsHeaderOnly})
#      set(${returnValue} false PARENT_SCOPE)
#      return()
#    endif()
#  endforeach()
#
#  set(${returnValue} true PARENT_SCOPE)
#
#
#  set(${return_value} "ahoj")
#endfunction()
#
#IS_LIBRARY_HEADER_ONLY(OJOJ "moje")
#message("${OJOJ}")
#
#message("${${LIB_NAME}_SUBPROJECTS}")

#
#
#
#if("${AS_SUBPROJECT}" STREQUAL "")
#  #IN_LIST is operator in CMake 3.3, CMake 3.8.1 warns 
#  cmake_policy(SET CMP0057 NEW)
#  list(APPEND AS_SUBPROJECT ${${LIB_NAME}_SUBPROJECTS})
#  macro(find_package name)
#    if(NOT "${name}" IN_LIST AS_SUBPROJECT)
#      _find_package(${ARGV})
#    endif()
#  endmacro()
#else()
#  list(APPEND AS_SUBPROJECT ${${LIB_NAME}_SUBPROJECTS})
#endif()
#
##build sub projects as static libs
#foreach(subProj ${${LIB_NAME}_BUILD_AS_STATIC})
#  set(subProjUpper)
#  string(TOUPPER ${subProj} subProjUpper)
#  set(${subProjUpper}_SHARED FALSE CACHE BOOL "")
#endforeach()
#
#
#
#
#
#
##if this library is build as static, and that it is linked to dynamic library it needs to be build with -fPIC flag
#if(NOT ${HEADER_ONLY_LIBRARY})
#  IF(CMAKE_COMPILER_IS_GNUCXX)
#    if(NOT ${LIB_NAME_UPPER}_SHARED)
#      target_compile_options(${LIB_NAME} PUBLIC -fPIC) # TODO how does windows handle this thing?
#    endif()
#  endif()
#endif()
#
#
#
#
#
#
#
#
#
#
#
#
#foreach(lib ${${LIB_NAME}_EXTERNAL_LIBRARIES_TO_FIND})
#  find_package(${lib} CONFIG)
#endforeach()
#
#if(NOT ${LIB_NAME}_OBJECT) #TODO give upper library _EXTERNAL_TARGETS
#  target_link_libraries (${LIB_NAME} ${INTERFACE_TYPE} ${${LIB_NAME}_EXTERNAL_TARGETS})
#endif()
#
#target_compile_definitions(${LIB_NAME} ${INTERFACE_TYPE} ${${LIB_NAME}_COMPILATION_OPTION})
#
##set building flags
##this should be specified elsewhere
#IF(NOT ${HEADER_ONLY_LIBRARY})
#  IF(CMAKE_COMPILER_IS_GNUCXX)
#    target_compile_options(${LIB_NAME} PRIVATE -W)
#    target_compile_options(${LIB_NAME} PRIVATE -Wall)
#    target_compile_options(${LIB_NAME} PRIVATE -Wconversion)
#    target_compile_options(${LIB_NAME} PRIVATE -pedantic)
#  ENDIF()
#  
#  IF(CMAKE_COMPILER_IS_GNUCC)
#    target_compile_options(${LIB_NAME} PRIVATE -W)
#    target_compile_options(${LIB_NAME} PRIVATE -Wall)
#    target_compile_options(${LIB_NAME} PRIVATE -Wconversion)
#    target_compile_options(${LIB_NAME} PRIVATE -pedantic)
#  ENDIF()
#ENDIF()
#
##construct library name in UPPER letters
#
