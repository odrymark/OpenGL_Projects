#pragma once
#include <glm.hpp>

class Game;

class Physics
{
public:
    Physics(Game* game = nullptr);

    void setGame(Game* g) { game = g; }

    void breakBlock();

    void jump();

    bool checkCollision(float playerHeight, float playerRadius, glm::vec3 cameraPos);

    float applyGravity(glm::vec3& cameraPos, float playerHeight, float deltaTime);

    void targetBlock(glm::vec3 cameraPos, glm::vec3 cameraFront);

    bool hasTarget;
    float breakTimer;
    glm::ivec3 targetedBlock;

private:
    const float MAX_RAY_DIST = 8.f;
    const float RAY_STEP = 0.05f;
    const float GRAVITY = -10.f;
    const float TERMINAL_VELOCITY = -20.f;
    const float JUMP_FORCE = 5.5f;
    const float BREAK_COOLDOWN = 1.f;

    Game* game;

    float yVelocity;
    bool isGrounded;

    bool checkGrounded(glm::vec3 cameraPos, float playerHeight);
};