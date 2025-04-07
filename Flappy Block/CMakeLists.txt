cmake_minimum_required(VERSION 3.28)
project(Flappy_Block)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_definitions(-DUNICODE -D_UNICODE)

add_executable(Flappy_Block main.cpp)

target_link_libraries(Flappy_Block gdi32)
