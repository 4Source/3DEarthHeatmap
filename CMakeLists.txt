cmake_minimum_required (VERSION 3.5)
project (3DEarthHeatmap)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)

add_executable(3DEarthHeatmap 
    src/main.cpp
)

