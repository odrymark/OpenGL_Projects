cmake_minimum_required(VERSION 4.1)
project(04_coordinate_sys)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external/glm
        /home/odrymark/glfw3-3.3.10/include
        /home/odrymark/glfw3-3.3.10/build
)

add_executable(04_coordinate_sys
        src/main.cpp
        src_glad/glad.c
        include/shader.h
        include/stb_image.h
)

target_link_libraries(04_coordinate_sys PRIVATE
        /home/odrymark/glfw3-3.3.10/build/src/libglfw3.a
        dl
        X11
        pthread
        Xrandr
        Xi
        Xxf86vm
)