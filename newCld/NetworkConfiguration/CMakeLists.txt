cmake_minimum_required(VERSION 3.10.1)

project(
  NetworkConfiguration
  VERSION 1.0.0
  )

#set these variables to *.cpp, *.c, ..., *.h, *.hpp, ...
set(SOURCES )
set(PRIVATE_INCLUDES )
set(PUBLIC_INCLUDES )
set(INTERFACE_INCLUDES 
  src/${PROJECT_NAME}/Fwd.h
  src/${PROJECT_NAME}/ActivationType.h
  src/${PROJECT_NAME}/Layer.h
  src/${PROJECT_NAME}/Input.h
  src/${PROJECT_NAME}/Conv2D.h
  src/${PROJECT_NAME}/Concatenate.h
  src/${PROJECT_NAME}/Bias.h
  src/${PROJECT_NAME}/Pooling.h
  src/${PROJECT_NAME}/PoolingType.h
  src/${PROJECT_NAME}/PaddingType.h
  src/${PROJECT_NAME}/GlobalMaxPooling.h
  src/${PROJECT_NAME}/BatchNormalization.h
  src/${PROJECT_NAME}/Dense.h
  src/${PROJECT_NAME}/Activation.h
  src/${PROJECT_NAME}/Network.h
  src/${PROJECT_NAME}/NetworkConfiguration.h
  )

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
set(ExternPublicLibraries )
set(ExternInterfaceLibraries Tensor\\ 1.0.0\\ CONFIG\\ REQUIRED)

#set these variables to targets
set(PrivateTargets )
set(PublicTargets )
set(InterfaceTargets Tensor::Tensor)

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
