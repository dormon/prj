cmake_minimum_required(VERSION 3.1.0)

PROJECT(libA)

add_library(basic fceB.cpp fceB.h)

add_subdirectory(libA)
