#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "world.h"
#include "physics.h"
#include "camera.h"
#include "shader.h"

class Game {
public:
    Game();
    void run(GLFWwindow* window);

    void processMouseInput(float xOffset, float yOffset) { camera.processMouseInput(xOffset, yOffset); }
    void processInput(GLFWwindow *window);

    void setBlock(int x, int y, int z, int value) { world.setBlock(x, y, z, value); }
    bool isBlockSolid(int x, int y, int z) const { return world.isBlockSolid(x, y, z); }
    bool isOutOfWorld(int x, int y, int z) { return world.isOutOfWorld(x, y, z); }

    int getSizeX() const { return world.WORLD_X; }
    int getSizeY() const { return world.WORLD_Y; }
    int getSizeZ() const { return world.WORLD_Z; }

    void setShader(Shader shaderProg);
    void setTexture(const unsigned int tex[3]);

private:
    Physics physics;
    World world;
    Camera camera;

    Shader shader;
    unsigned int texture[3]{};

    float deltaTime = 0.f;
    float lastFrame = 0.f;

    static constexpr float PLAYER_HEIGHT = 1.2f;
    static constexpr float PLAYER_RADIUS = 0.2f;

    void renderBlock(int x, int y, int z) const;
};
