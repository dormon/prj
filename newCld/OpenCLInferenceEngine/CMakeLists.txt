cmake_minimum_required(VERSION 3.10.1)

project(
  OpenCLInferenceEngine
  VERSION 1.0.0
  )

#set these variables to *.cpp, *.c, ..., *.h, *.hpp, ...
set(SOURCES 
  src/${PROJECT_NAME}/Engine.cpp
  src/${PROJECT_NAME}/Kernels.cpp
  src/${PROJECT_NAME}/OpenCLLayers.cpp
  )
set(PRIVATE_INCLUDES )
set(PUBLIC_INCLUDES 
  src/${PROJECT_NAME}/Engine.h
  src/${PROJECT_NAME}/OpenCLHelper.h
  src/${PROJECT_NAME}/Kernels.h
  src/${PROJECT_NAME}/OpenCLLayers.h
  )
set(INTERFACE_INCLUDES )

#list of libraries to find
#it should contain list of "lists"
#format:
#set(ExternLibraries
#  A\\ B\\ C\\ D
#  E\\ F\\ G
#  )
#It will be converted into:
#find_package(A B C D)
#find_package(E F G)
#If version is specified, it has to be the second parameter (B)
set(ExternPrivateLibraries )
set(ExternPublicLibraries 
  InferenceEngine\\ 1.0.0\\ CONFIG\\ REQUIRED
  OpenCLLists\\ 1.0.0\\ CONFIG\\ REQUIRED
  TypeList\\ 1.0.0\\ CONFIG\\ REQUIRED
  )
set(ExternInterfaceLibraries )

#set these variables to targets
set(PrivateTargets )
set(PublicTargets InferenceEngine::InferenceEngine OpenCLLists::OpenCLLists TypeList::TypeList )
set(InterfaceTargets )

#set these libraries to variables that are provided by libraries that does not support configs
set(PrivateIncludeVariables )
set(PublicIncludeVariables )
set(InterfaceIncludeVariables )
set(PrivateReleaseLibraryVariables )
set(PublicReleaseLibraryVariables )
set(InterfaceReleaseLibraryVariables )
set(PrivateDebugLibraryVariables )
set(PublicDebugLibraryVariables )
set(InterfaceDebugLibraryVariables )

include(../CMakeLibrary.cmake)
