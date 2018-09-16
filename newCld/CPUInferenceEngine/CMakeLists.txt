cmake_minimum_required(VERSION 3.10.1)

project(
  CPUInferenceEngine
  VERSION 1.0.0
  )

#set these variables to *.cpp, *.c, ..., *.h, *.hpp, ...
set(SOURCES src/${PROJECT_NAME}/Engine.cpp)
set(PRIVATE_INCLUDES )
set(PUBLIC_INCLUDES src/${PROJECT_NAME}/Engine.h)
set(INTERFACE_INCLUDES)

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
  CPULayerImplementation\\ 1.0.0\\ CONFIG\\ REQUIRED
  )
set(ExternInterfaceLibraries )

#set these variables to targets
set(PrivateTargets )
set(PublicTargets InferenceEngine::InferenceEngine CPULayerImplementation::CPULayerImplementation)
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
