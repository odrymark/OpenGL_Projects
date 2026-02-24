cmake_minimum_required(VERSION 4.1)
project(Minecraft_Clone)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external/glm
        /home/odrymark/glfw3-3.3.10/include
        /home/odrymark/glfw3-3.3.10/build
)

add_executable(Minecraft_Clone
        src/main.cpp
        src_glad/glad.c
        include/shader.h
        include/stb_image.h
        include/camera.h
        include/physics.h
        src/physics.cpp
        include/world.h
        src/world.cpp
        include/game.h
        src/game.cpp
)

target_link_libraries(Minecraft_Clone PRIVATE
        /home/odrymark/glfw3-3.3.10/build/src/libglfw3.a
        dl
        X11
        pthread
        Xrandr
        Xi
        Xxf86vm
)