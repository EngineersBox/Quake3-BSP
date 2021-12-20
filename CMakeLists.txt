cmake_minimum_required(VERSION 3.10)

project(Quake VERSION 1.0)

# Debug
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O1")

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include source content
file (GLOB_RECURSE Quake_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE Quake_HEADERS CONFIGURE_DEPENDS "src/*.h")

set (Quake_INCLUDE_DIRS "")
foreach (_headerFile ${Quake_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Quake_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES Quake_INCLUDE_DIRS)

# Mark executable
add_executable(Quake ${Quake_SOURCES})
target_include_directories(Quake PRIVATE ${Quake_INCLUDE_DIRS})

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

# Configure Glm
find_package(glm CONFIG REQUIRED)
target_link_libraries(Quake PRIVATE glm::glm)

#Configure spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(Quake PRIVATE spdlog::spdlog spdlog::spdlog_header_only)