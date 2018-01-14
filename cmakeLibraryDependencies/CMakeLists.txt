cmake_minimum_required(VERSION 3.1.0)

project(libraryDependencies)

add_library(headerA         INTERFACE                        )
target_include_directories(headerA INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/headerA>)

add_library(headerB_headerA INTERFACE                        )
add_library(headerC_staticA INTERFACE                        )
add_library(headerC_sharedA INTERFACE                        )

add_library(staticA STATIC    A/A.cpp A/A.h)
add_library(sharedA SHARED    A/A.cpp A/A.h)

add_library(staticB_private___headerA STATIC    B_headerA/B_headerA.cpp B_headerA/B_headerA.h)
add_library(staticB_public____headerA STATIC    B_headerA/B_headerA.cpp B_headerA/B_headerA.h)
#add_library(staticB_interface_headerA STATIC    B_headerA/B_headerA.cpp B_headerA/B_headerA.h)

add_library(sharedB_private___headerA SHARED    B_headerA/B_headerA.cpp B_headerA/B_headerA.h)
add_library(sharedB_public____headerA SHARED    B_headerA/B_headerA.cpp B_headerA/B_headerA.h)
#add_library(sharedB_interface_headerA SHARED    B_headerA/B_headerA.cpp B_headerA/B_headerA.h)


#header on header
#target_link_libraries(headerB_headerA PRIVATE   headerA)
#target_link_libraries(headerB_headerA PUBLIC    headerA)
target_link_libraries(headerB_headerA INTERFACE headerA)

#header on static
#target_link_libraries(headerC_staticA PRIVATE   staticA)
#target_link_libraries(headerC_staticA PUBLIC    staticA)
target_link_libraries(headerC_staticA INTERFACE staticA)

#header on shared
#target_link_libraries(headerC_sharedA PRIVATE   sharedA)
#target_link_libraries(headerC_sharedA PUBLIC    sharedA)
target_link_libraries(headerC_sharedA INTERFACE sharedA)

#static on header
target_link_libraries(staticB_private___headerA PRIVATE   headerA)
target_link_libraries(staticB_public____headerA PUBLIC    headerA)
#target_link_libraries(staticB_interface_headerA INTERFACE headerA)

##shared on header
target_link_libraries(sharedB_private___headerA PRIVATE   headerA)
target_link_libraries(sharedB_public____headerA PUBLIC    headerA)
#target_link_libraries(sharedB_interface_headerA INTERFACE headerA)

