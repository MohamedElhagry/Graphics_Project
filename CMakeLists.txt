cmake_minimum_required(VERSION 3.16)
project(Graphics_Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(Graphics_Project main.cpp DrawSimpleShapes.h fillingQuarter.h hermitFilling.h nonConvex.h convex.h "Clipping algorithms.h" "Filling Utilities.h")
target_link_libraries(Graphics_Project -lOpenGL32  -lGLU32)
