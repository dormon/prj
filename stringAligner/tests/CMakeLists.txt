cmake_minimum_required(VERSION 3.10.1)

add_executable(tests TestsMain.cpp TextTests.cpp TableTests.cpp CellTests.cpp ComplexTests.cpp catch.hpp)

target_link_libraries(tests StringAligner::StringAligner)

