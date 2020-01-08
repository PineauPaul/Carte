cmake_minimum_required(VERSION 2.6)
project(unittest) 

enable_testing()

add_library(test_main OBJECT test_main.cpp)

add_executable(unittest_exe test.cpp Math.cpp $<TARGET_OBJECTS:test_main>)

add_test(NAME unittest_exe COMMAND unittest_exe)
