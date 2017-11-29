cmake_minimum_required(VERSION 3.1.0)

PROJECT(fceA)

add_library(basic fceA.cpp fceA.h)

add_subdirectory(libA)
