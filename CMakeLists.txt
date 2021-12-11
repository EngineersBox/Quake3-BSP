cmake_minimum_required(VERSION 3.10)

project(Quake VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Mark executable
add_executable(Quake src/g_main.cpp "src/resource/loader/QLoader.h" "src/resource/loader/QLoader.cpp" "src/core/window.cpp" "src/core/window.h" )

# Configure SDL2
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Quake PUBLIC ${SDL2_LIBRARIES})
target_include_directories(Quake PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(Quake PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(Quake PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Configure SDL2_Image
find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(Quake PRIVATE SDL2::SDL2_image)

# Configure OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(Quake PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(Quake PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(Quake PUBLIC ${OPENGL_LIBRARIES})