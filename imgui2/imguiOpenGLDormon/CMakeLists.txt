cmake_minimum_required(VERSION 3.10.1)

project(
  imguiOpenGLDormon
  VERSION 1.0.0
  )

SET(CMAKE_CXX_STANDARD 14)

find_package(imguiDormon CONFIG REQUIRED)

set(SOURCES
  src/${PROJECT_NAME}/BackupOpenGLState.cpp
  src/${PROJECT_NAME}/GetProcAddress.cpp
  src/${PROJECT_NAME}/OpenGLTable.cpp
  src/${PROJECT_NAME}/Renderer.cpp
  src/${PROJECT_NAME}/RendererImpl.cpp
  )

set(INCLUDES
  src/${PROJECT_NAME}/Renderer.h
  )

set(PRIVATE_INCLUDES
  src/${PROJECT_NAME}/GetProcAddress.h
  src/${PROJECT_NAME}/MinimalOpenGL.h
  src/${PROJECT_NAME}/BackupOpenGLState.h
  src/${PROJECT_NAME}/MinimalOpenGL.h
  src/${PROJECT_NAME}/OpenGLTable.h
  src/${PROJECT_NAME}/RendererImpl.h
  )

option(BUILD_SHARED_LIBS "build this library as shared")

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

add_library(${PROJECT_NAME} ${SOURCES} ${INCLUDES} ${PRIVATE_INCLUDES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(GNUInstallDirs)

target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

target_link_libraries(${PROJECT_NAME} PUBLIC imguiDormon::imguiDormon)

set(PROJECT_NAME_LOWER)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION)


install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(
  FILES
  ${INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h
  DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  COMPONENT
  Devel
  )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

export(EXPORT ${PROJECT_NAME}Targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  )

set(ConfigPackageLocation lib/cmake/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME}Targets
  FILE
  ${PROJECT_NAME}Targets.cmake
  NAMESPACE
  ${PROJECT_NAME}::
  DESTINATION
  ${ConfigPackageLocation}
  )

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
  "include(CMakeFindDependencyMacro)\n"
  "find_dependency(imguiDormon)\n"
  "include($" "{CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)"
  )

install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION
  ${ConfigPackageLocation}
  COMPONENT
  Devel
  )

