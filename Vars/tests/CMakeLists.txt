cmake_minimum_required(VERSION 3.13.0)

add_subdirectory(Catch2-3.1.1)
add_executable(tests TestsMain.cpp tests.cpp)

target_link_libraries(tests Vars::Vars Catch2::Catch2WithMain)
