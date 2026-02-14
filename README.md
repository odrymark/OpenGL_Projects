# OpenGL Projects

A collection of my OpenGL learning projects and studys.

This repository showcases various OpenGL concepts and techniques I've implemented while learning modern OpenGL (3.3 core profile).

## Projects

| Folder                  | Description                                      | Key Topics / Techniques                          | Status    |
|-------------------------|--------------------------------------------------|--------------------------------------------------|-----------|
| `01_triangle`           | Basic colored triangle                           | VAO, VBO, Shaders, Basic Pipeline                | Completed |
| `02_textures`           | Basic textured rectangle                         | Textures, EBO                                    | Completed |
| `03_transformation`     | Constantly rotating rectangle                    | Matrices, Dot Products, Transformation           | Completed |
| `04_coordinate_sys`     | Constantly rotating 3D cubes                     | Spaces, Projections, Model-View-Projection Matrices | Completed |

## Technologies Used

- **Language**: C++17 / C++20
- **OpenGL**: 3.3 Core Profile
- **Window & Context**: GLFW
- **Math library**: GLM
- **Image loading**: stb_image
- **Build system**: CMake

## How to Build (most projects)

```bash
cd 01_triangle
mkdir build && cd build
cmake ..
cmake --build .
