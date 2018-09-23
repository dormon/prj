cmake_minimum_required(VERSION 3.12.1)

project(FastNoise)

SET(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(REVISION_VERSION 0)

#include(GenerateExportHeader)

find_package(geGL 1.0.5 CONFIG REQUIRED)
find_package(SDL2CPP 1.0 CONFIG REQUIRED)
find_package(imguiSDL2OpenGL CONFIG REQUIRED)
find_package(Vars CONFIG REQUIRED)


set(SOURCES
  src/main.cpp
  )

set(INCLUDES
  )

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2CPP::SDL2CPP geGL::geGL imguiSDL2OpenGL::imguiSDL2OpenGL Vars::Vars)

function(getSharedLibraries out lib cfg)
  #message("getSharedLibraries(${lib} ${cfg})")

  if(TARGET ${lib})
    get_target_property(target_type ${lib} TYPE)
    if(${target_type} STREQUAL "INTERFACE_LIBRARY")
      #message("${lib} JE ${target_type}")
      return()
    endif()
  else()
    #message("${lib} NENI TARGET")
    return()
  endif()


  get_target_property(dll ${lib} IMPORTED_LOCATION_${cfg})
  list(APPEND dlls ${dll})

  get_target_property(interfaceLibs ${lib} INTERFACE_LINK_LIBRARIES)
  if(NOT "${interfaceLibs}" STREQUAL "interfaceLibs-NOTFOUND")
    foreach(interfaceLib ${interfaceLibs})
      getSharedLibraries(ilibs ${interfaceLib} ${cfg})
      list(APPEND dlls ${ilibs})
    endforeach()
  endif()
  list(REMOVE_DUPLICATES dlls)
  set(${out} ${dlls} PARENT_SCOPE)
endfunction()


function(getAllSharedLibraries allLibraries app cfg)
  get_target_property(libs ${app} LINK_LIBRARIES)
  foreach(lib ${libs})
    getSharedLibraries(libList ${lib} ${cfg})
    #message("${lib} ###### ${libList}")
    list(APPEND allLibs ${libList})
  endforeach()
  list(REMOVE_DUPLICATES allLibs)
  set(${allLibraries} ${allLibs} PARENT_SCOPE)
endfunction()

getAllSharedLibraries(allDebugSharedLibraries ${PROJECT_NAME} DEBUG)
getAllSharedLibraries(allReleaseSharedLibraries ${PROJECT_NAME} RELEASE)

#message("SharedDebug  : ${allDebugSharedLibraries}")
#message("SharedRelease: ${allReleaseSharedLibraries}")

if(MSVC)
  foreach(lib ${allDebugSharedLibraries})
    file(COPY ${lib} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
  endforeach()
  foreach(lib ${allReleaseSharedLibraries})
    file(COPY ${lib} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
  endforeach()
endif() 
