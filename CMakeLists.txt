cmake_minimum_required(VERSION 2.8.3)

project(carte)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)



include_directories(include)

add_library(carte src/Kart.cpp)
add_library(Math src/Math.cpp)


add_executable(create_raster src/main.cpp)
target_link_libraries(create_raster carte Math)


