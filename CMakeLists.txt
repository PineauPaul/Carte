cmake_minimum_required(VERSION 2.8.3)
project(carte)


include_directories(src)


add_executable(main src/main2.cpp)

target_link_libraries(main)

