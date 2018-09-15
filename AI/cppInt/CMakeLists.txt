cmake_minimum_required(VERSION 3.1.0)

set(APPLICATION_NAME domain)
PROJECT(${APPLICATION_NAME})


set(SOURCES
  src/CommandInput.cpp
  src/CommandDescription.cpp
  src/AtomicCommandDescription.cpp
  src/BodyCommandDescription.cpp
  src/ModelDescription.cpp
  src/AtomicModelDescription.cpp
  src/VectorModelDescription.cpp
  src/CompositeModelDescription.cpp
  src/Domain.cpp
  src/main.cpp
  )

set(INCLUDES
  include/CommandInput.h
  include/CommandDescription.h
  include/AtomicCommandDescription.h
  include/BodyCommandDescription.h
  include/ModelDescription.h
  include/AtomicModelDescription.h
  include/VectorModelDescription.h
  include/CompositeModelDescription.h
  include/Domain.h
  )

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

IF(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-W          )
  add_compile_options(-Wall       )
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic   )
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(-W)
  add_compile_options(-Wall)
  add_compile_options(-Wconversion)
  add_compile_options(-pedantic)
ENDIF()


add_executable(${APPLICATION_NAME} ${SOURCES} ${INCLUDES})

include_directories(${${APPLICATION_NAME}_SOURCE_DIR}/include)
