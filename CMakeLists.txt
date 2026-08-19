cmake_minimum_required(VERSION 3.20)

project(GalaxyViewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(galaxy_viewer
    src/main.cpp
    src/Camera.cpp
    src/Geometry.cpp
    src/Renderer.cpp
    src/Shader.cpp
    external/glad/src/gl.c
)

find_package(glfw3 3.4 REQUIRED)

target_link_libraries(galaxy_viewer PRIVATE
    glfw
)

target_include_directories(galaxy_viewer PRIVATE
    external/glad/include
    src
)
