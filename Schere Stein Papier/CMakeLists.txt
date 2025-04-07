cmake_minimum_required(VERSION 3.25)
project(RetroGame)

set(CMAKE_CXX_STANDARD 17)

target_link_libraries(RetroGame gdi32)

add_executable(RetroGame main.cpp)
