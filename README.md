# OpenGL Projects

A collection of my OpenGL learning projects and studys.

This repository showcases various OpenGL concepts and techniques I've implemented while learning modern OpenGL (3.3 core profile).

## Projects

| Folder                  | Description                                      | Key Topics / Techniques                          | Status    |
|-------------------------|--------------------------------------------------|--------------------------------------------------|-----------|
| `01_triangle`           | Basic colored triangle                           | VAO, VBO, shaders, basic pipeline                | Complete  |
| `02_textures`           | Basic textured rectangle                         | Textures                                         | Ongoing   |

## Technologies Used

- **Language**: C++17 / C++20
- **OpenGL**: 3.3 Core Profile
- **Window & Context**: GLFW
- **Build system**: CMake

## How to Build (most projects)

```bash
# Example for a single project folder
cd 01_triangle
mkdir build && cd build
cmake ..
cmake --build .    # or make / ninja / open .sln
