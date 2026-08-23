cmake_minimum_required(VERSION 3.20)

project(GalaxyViewer LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

add_executable(galaxy_viewer
    src/main.cpp
    src/Camera.cpp
    src/Geometry.cpp
    src/GeometryBuilder.cpp
    src/Renderer.cpp
    src/Scene.cpp
    src/Shader.cpp
    src/star/StarGenerator.cpp
    external/glad/src/gl.c
)

target_link_libraries(galaxy_viewer PRIVATE
    glfw
)

target_include_directories(galaxy_viewer PRIVATE
    external/glad/include
    external/glm
    src
)

target_link_options(galaxy_viewer PRIVATE
    -static-libgcc
    -static-libstdc++
)
