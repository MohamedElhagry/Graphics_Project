cmake_minimum_required(VERSION 3.16)
project(Graphics_Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(Graphics_Project main.cpp fillingQuarter.cpp nonConvex.cpp convex.cpp bezierFilling.cpp hermitFillling.cpp "Filling Utilities.cpp")
target_link_libraries(Graphics_Project -lOpenGL32  -lGLU32)
