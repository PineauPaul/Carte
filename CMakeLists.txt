cmake_minimum_required(VERSION 2.8.3)
project(carte)


include_directories(src)


add_executable(create_raster src/main.cpp)

target_link_libraries(create_raster)

