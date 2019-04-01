cmake_minimum_required(VERSION 3.13)

find_package(libb 1.0 CONFIG REQUIRED)

include(print_target_properties.cmake)

add_executable(dormon main.cpp)
target_link_libraries(dormon libb::libb)

#print_target_properties(libb::libb)
#print_target_properties(liba::liba)
print_target_properties(dormon)

set(WHITE_PROPS COMPATIBLE_INTERFACE_STRING IMPORTED IMPORTED_GLOBAL INTERFACE_INCLUDE_DIRECTORIES NAME TYPE IMPORTED_LOCATION)

foreach (prop ${WHITE_PROPS})
  get_target_property(propval dormon ${prop})
  message ("dormon ${prop} = ${propval}")
endforeach()
