# Sets minimum cmake version required for project
cmake_minimum_required(VERSION 3.20)

# Names project
project(OrbitalSimulation)

# Source Files
add_executable(main src/main.cpp)

# Searches for and ensures raylib headers are included
# CMAKE_SOURCE_DIR = top-level folder (project root)
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/include
)

# Connects build to raylib's compiled libs
target_link_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/raylib/lib
)

# Link Raylib compiled code + Windows system libs
target_link_libraries(main
    raylib
    opengl32
    gdi32
    winmm
)