cmake_minimum_required(VERSION 3.20)
project(MyEngine CXX)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3
find_package(SDL3 REQUIRED)


# Explicitly list source files instead of GLOB
add_executable(MyEngine
    src/main.cpp
)

# Include directories
target_include_directories(MyEngine PRIVATE 
    src/
)

# Link SDL3
target_link_libraries(MyEngine PRIVATE SDL3::SDL3)