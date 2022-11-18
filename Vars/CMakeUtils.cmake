set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

if("${SOURCES}" STREQUAL "")
  set(HeaderOnly TRUE)
else()
  set(HeaderOnly FALSE)
endif()

#find private dependencies
foreach(lib ${ExternPrivateLibraries})
  list(GET lib 0 libName)
  if(NOT TARGET ${libName}) 
    find_package(${lib})
  endif()
endforeach()

#find public dependencies
foreach(lib ${ExternPublicLibraries})
  list(GET lib 0 libName)
  if(NOT TARGET ${libName}) 
    find_package(${lib})
  endif()
endforeach()


if(NOT ${HeaderOnly})
  option(BUILD_SHARED_LIBS "build this library as shared")
endif()

SET(CMAKE_DEBUG_POSTFIX          "d"  CACHE STRING "add a postfix, usually d on windows"    )
SET(CMAKE_RELEASE_POSTFIX        ""   CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX     "s"  CACHE STRING "add a postfix, usually empty on windows")

if(${HeaderOnly})
  add_library(${PROJECT_NAME} INTERFACE)
else()
  add_library(${PROJECT_NAME} ${SOURCES} ${PRIVATE_INCLUDES} ${PUBLIC_INCLUDES} ${INTERFACE_INCLUDES})
endif()

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(GNUInstallDirs)

if(${HeaderOnly})
  target_include_directories(${PROJECT_NAME} INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
else()
  target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
endif()

foreach(inc ${PrivateIncludeVariables})
  target_include_directories(${PROJECT_NAME} PRIVATE ${${inc}})
endforeach()

foreach(inc ${PublicIncludeVariables})
  #this has to be also private because we are exporting this include manually
  target_include_directories(${PROJECT_NAME} PRIVATE ${${inc}})
endforeach()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

if(${HeaderOnly})
  target_link_libraries(${PROJECT_NAME} 
    INTERFACE ${InterfaceTargets}
  )
else()
  target_link_libraries(${PROJECT_NAME} 
    PUBLIC    ${PublicTargets} 
    PRIVATE   ${PrivateTargets}
    INTERFACE ${InterfaceTargets}
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
  foreach(lib ${PrivateReleaseLibraryVariables})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${${lib}})
  endforeach()
  foreach(lib ${PublicReleaseLibraryVariables})
    #this has to be also PRIVATE, we are exporting manually...
    target_link_libraries(${PROJECT_NAME} PRIVATE ${${lib}})
  endforeach()
else()
    foreach(lib ${PrivateDebugLibraryVariables})
      target_link_libraries(${PROJECT_NAME} PRIVATE ${${lib}})
    endforeach()
    foreach(lib ${PublicDebugLibraryVariables})
      #this has to be also PRIVATE, we are exporting manually...
      target_link_libraries(${PROJECT_NAME} PRIVATE ${${lib}})
    endforeach()
endif()




set(PROJECT_NAME_LOWER)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

if(NOT ${HeaderOnly})
  include(GenerateExportHeader)
  generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h)
  set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})
  set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION ${PROJECT_VERSION_MAJOR})
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

#install header files
if(NOT ${HeaderOnly})
  install(
    FILES       ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME_LOWER}_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT   Devel
    )
endif()

install(
  FILES       ${PUBLIC_INCLUDES} ${INTERFACE_INCLUDES}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  COMPONENT   Devel
  )

#create *ConfigVersion.cmake config file
include(CMakePackageConfigHelpers)
set(ConfigDir ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
write_basic_package_version_file(
  ${ConfigDir}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion
  )

#create *Targets.cmake config file
export(EXPORT ${PROJECT_NAME}Targets
  FILE      ${ConfigDir}/${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  )

#create content of *Config.cmake config file
string(CONCAT ConfigContent
  "include(CMakeFindDependencyMacro)\n"
  )

foreach(lib ${ExternPublicLibraries} ${ExternInterfaceLibraries})
  list(GET lib 0 libName)
  list(LENGTH lib len)
  if(${len} LESS 2)
    string(CONCAT ConfigContent
      ${ConfigContent}
      "find_dependency(" ${libName} ")\n"
      )
  else()
    list(GET lib 1 libVersion)
    #try to match the version
    string(REGEX MATCH "^[0-9]+(\\.[0-9]+)*$" matchedVersion ${libVersion})
    if("${matchedVersion}" STREQUAL "")
      string(CONCAT ConfigContent
        ${ConfigContent}
        "find_dependency(" ${libName} ")\n"
        )
    else()
      string(CONCAT ConfigContent
        ${ConfigContent}
        "find_dependency(" ${libName} " " ${libVersion} ")\n"
        )
    endif()
  endif()
endforeach()


string(CONCAT ConfigContent
  ${ConfigContent}
  "include($" "{CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)\n"
  )

#create *Config.cmake config file
file(WRITE ${ConfigDir}/${PROJECT_NAME}Config.cmake ${ConfigContent})

#install configs
set(ConfigPackageLocation lib/cmake/${PROJECT_NAME})
install(
  FILES
  ${ConfigDir}/${PROJECT_NAME}Config.cmake
  ${ConfigDir}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${ConfigPackageLocation}
  COMPONENT   Devel
  )

install(EXPORT ${PROJECT_NAME}Targets
  FILE        ${PROJECT_NAME}Targets.cmake
  NAMESPACE   ${PROJECT_NAME}::
  DESTINATION ${ConfigPackageLocation}
  )

if((NOT "${PublicIncludeVariables}" STREQUAL "") OR (NOT "${InterfaceIncludeVariables}" STREQUAL ""))

  string(CONCAT FIX
    "\n"
    "# We need to fix libraries that do not provide configs but variables (like GLEW_INCLUDE_DIR GLEW_LIBRARY_RELEASE)\n"
    "# We do it by adding these variables to INTERFACE_INCLUDE_DIRECTORIES and INTERFACE_LINK_LIBRARIES\n"
    "get_target_property(includes " ${PROJECT_NAME} "::" ${PROJECT_NAME} " INTERFACE_INCLUDE_DIRECTORIES)\n"
    "get_target_property(libs " ${PROJECT_NAME} "::" ${PROJECT_NAME} " INTERFACE_LINK_LIBRARIES)\n"
    )
  
  
  foreach(inc ${PublicIncludeVariables})
    string(CONCAT FIX "${FIX}"
      "string(CONCAT includes \\\"\\$" "{includes}\\\" \\\"\;\\$" "{${inc}}\\\")\n"
      )
  endforeach()
  
  foreach(inc ${InterfaceIncludeVariables})
    string(CONCAT FIX "${FIX}"
      "string(CONCAT includes \\\"\\$" "{includes}\\\" \\\"\;\\$" "{${inc}}\\\")\n"
      )
  endforeach()
  
  foreach(lib ${PublicReleaseLibraryVariables})
    string(CONCAT FIX "${FIX}"
      "string(CONCAT libs \\\"\\$" "{libs}\\\" \\\"\\$<\\$<CONFIG:Release>:\;\\$" "{${lib}}>\\\")\n"
      )
  endforeach()
  
  foreach(lib ${InterfaceReleaseLibraryVariables})
    string(CONCAT FIX "${FIX}"
      "string(CONCAT libs \\\"\\$" "{libs}\\\" \\\"\\$<\\$<CONFIG:Release>:\;\\$" "{${lib}}>\\\")\n"
      )
  endforeach()
  
  foreach(lib ${PublicDebugLibraryVariables})
    string(CONCAT FIX "${FIX}"
      "string(CONCAT libs \\\"\\$" "{libs}\\\" \\\"\\$<\\$<CONFIG:Debug>:\;\\$" "{${lib}}>\\\")\n"
      )
  endforeach()
  
  foreach(lib ${InterfaceDebugLibraryVariables})
    string(CONCAT FIX "${FIX}"
      "string(CONCAT libs \\\"\\$" "{libs}\\\" \\\"\\$<\\$<CONFIG:Debug>:\;\\$" "{${lib}}>\\\")\n"
      )
  endforeach()
  
  string(CONCAT FIX "${FIX}"
    "set_target_properties(" ${PROJECT_NAME} "::" ${PROJECT_NAME} " PROPERTIES INTERFACE_INCLUDE_DIRECTORIES \\\"\\$" "{includes}\\\" INTERFACE_LINK_LIBRARIES \\\"\\$" "{libs}\\\")\n"
    )
  
  string(CONCAT installCode 
    "file(APPEND ${CMAKE_INSTALL_PREFIX}/${ConfigPackageLocation}/${PROJECT_NAME}Targets.cmake \"${FIX}\")\n"
    )
  
  install(CODE ${installCode})

endif()
