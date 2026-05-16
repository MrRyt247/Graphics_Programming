cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_ROOT "include/glfw")
set(GLFW_BUILD "include/glfw/build")
set(GLAD_ROOT "include/glad")
set(ASSIMP_ROOT "include/assimp")

find_package(OpenGL REQUIRED)

add_executable(app ${GLAD_ROOT}/src/glad.c src/main.cpp)

target_include_directories(app PRIVATE
    ${GLFW_ROOT}/include
    ${GLAD_ROOT}/include
    ${ASSIMP_ROOT}/include
)

target_link_libraries(app PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    ${CMAKE_SOURCE_DIR}/lib/libassimp.so
    OpenGL::GL
    X11
    Xrandr
    Xinerama
    Xcursor
    Xi
    pthread
    dl
)