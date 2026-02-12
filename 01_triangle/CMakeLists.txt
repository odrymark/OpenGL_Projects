cmake_minimum_required(VERSION 4.1)
project(first)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        /home/odrymark/glfw3-3.3.10/include
        /home/odrymark/glfw3-3.3.10/build
)

add_executable(first
        src/main.cpp
        src_glad/glad.c
        src/shader.h
)

target_link_libraries(first PRIVATE
        /home/odrymark/glfw3-3.3.10/build/src/libglfw3.a
        dl
        X11
        pthread
        Xrandr
        Xi
        Xxf86vm
)
