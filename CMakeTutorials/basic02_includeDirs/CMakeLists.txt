cmake_minimum_required(VERSION 3.1.0)

PROJECT(basic)

#promenna
set(SOURCES
  main.cpp
  )



#promenna
set(INCLUDES
  include/fwd.h
  include/MyClass.h
  )

#CMake automaticky zjisti zavislosti mezi soubory main.cpp na MyClass.h. MyClass.h na fwd.h
#Pokud se modifikuje fwd.h CMake vynuti preklad main.cpp

add_executable(basic ${SOURCES} ${INCLUDES})
target_include_directories(basic PUBLIC include/)
