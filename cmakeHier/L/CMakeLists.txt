cmake_minimum_required(VERSION 3.10.1)

project(ComposedLibrary)

add_subdirectory(A)
add_subdirectory(B)
add_subdirectory(C)

function(addSubSources libName)
  set(sources $<TARGET_PROPERTY:${libName},INTERFACE_SOURCES>)
  foreach(src ${sources})
    message("################################## ${libName}/${src}")
    target_sources(L PUBLIC ${libName}/${src})
  endforeach()
endfunction()


add_library(L)
target_sources(L PUBLIC 
  $<TARGET_PROPERTY:A,INTERFACE_SOURCES> $<TARGET_OBJECTS:A> 
  $<TARGET_PROPERTY:B,INTERFACE_SOURCES> $<TARGET_OBJECTS:B>
  $<TARGET_PROPERTY:C,INTERFACE_SOURCES> $<TARGET_OBJECTS:C>
  )

#addSubSources(A)
#addSubSources(B)
#addSubSources(C)
