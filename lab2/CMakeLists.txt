cmake_minimum_required(VERSION 3.10)

project("lab_2")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_subdirectory(googletest)

add_library(src src/Seven.cpp src/Seven.h)

add_executable(main main.cpp)
target_link_libraries(main src)

add_executable(tests tests/tests.cc)
target_link_libraries(tests src gtest_main)