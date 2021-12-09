cmake_minimum_required(VERSION 3.10)

project(Quake VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Mark executable
add_executable(Quake src/g_main.cpp "src/resource/loader/QLoader.h" "src/resource/loader/QLoader.cpp")

# Configure SDL2
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Quake PRIVATE SDL2::SDL2 SDL2::SDL2main)