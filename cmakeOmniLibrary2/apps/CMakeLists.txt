cmake_minimum_required(VERSION 3.10.1)

#IMPORTANT!!!!!!!!
#This will define list of subproject and will redefine find_package funcion using macro.
#Due to CMake bug: redefinition of any function multiple times crashes CMake configuration.
#It is essential to redefine find_package macro only once per super project.
#Each subproject tries to redefine find_package macro when variable as_subproject is empty.
#Variable as_subproject should contain list of subprojects.
#Redefined find_package function ignores packages that are subprojects (subprojects do not need to be found).
#This project contains one subproject - Inference
#Inference subproject is super-project that is composed of other sub-projects.
set(_SUBPROJECTS )
if("${AS_SUBPROJECT}" STREQUAL "")
  #IN_LIST is operator in CMake 3.3, CMake 3.8.1 warns 
  cmake_policy(SET CMP0057 NEW)
  list(APPEND AS_SUBPROJECT ${${LIB_NAME}_SUBPROJECTS})
  macro(find_package name)
    if(NOT "${name}" IN_LIST AS_SUBPROJECT)
      _find_package(${ARGV})
    endif()
  endmacro()
else()
  list(APPEND AS_SUBPROJECT ${${LIB_NAME}_SUBPROJECTS})
endif()

#add_subdirectory(HeaderApp)
#add_subdirectory(StaticApp)
#add_subdirectory(SharedApp)
#add_subdirectory(SharedCompositeApp)
#add_subdirectory(SharedLibraryOnSharedLibraryApp)

