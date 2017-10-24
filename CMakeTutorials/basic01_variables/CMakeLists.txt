cmake_minimum_required(VERSION 3.1.0)

PROJECT(basic)

#promenna
set(SOURCES
  main.cpp
  )

#promenna
set(INCLUDES
  fwd.h
  MyClass.h
  )

#CMake automaticky zjisti zavislosti mezi soubory main.cpp na MyClass.h. MyClass.h na fwd.h
#Pokud se modifikuje fwd.h CMake vinuti preklad main.cpp

add_executable(basic ${SOURCES} ${INCLUDES})
