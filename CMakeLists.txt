cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_ROOT "/usr/local/lib/glfw")
set(GLFW_BUILD "/usr/local/lib/glfw/build")
set(GLAD_ROOT "/usr/local/lib/glad")
set(ASSIMP_ROOT "/usr/local/lib/assimp")

find_package(OpenGL REQUIRED)

add_executable(app ${GLAD_ROOT}/src/glad.c src/main.cpp)

target_include_directories(app PRIVATE
    ${GLFW_ROOT}/include
    ${GLAD_ROOT}/include
    ${ASSIMP_ROOT}/include
)

target_link_libraries(app PRIVATE
    ${GLFW_BUILD}/src/libglfw3.a
    ${ASSIMP_ROOT}/bin/libassimp.so
    OpenGL::GL
    X11
    Xrandr
    Xinerama
    Xcursor
    Xi
    pthread
    dl

)