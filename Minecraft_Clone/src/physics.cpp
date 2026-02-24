#include "physics.h"
#include <glm.hpp>
#include <cmath>

#include "game.h"

Physics::Physics(Game* g) : game(g)
{
    breakTimer = 0.f;
    yVelocity = 0.f;
    isGrounded = false;
    hasTarget = false;
    targetedBlock = glm::ivec3(-1);
}

void Physics::targetBlock(glm::vec3 cameraPos, glm::vec3 cameraFront)
{
    hasTarget = false;

    glm::vec3 rayOrigin = cameraPos + cameraFront * 0.1f;
    glm::vec3 rayDir = glm::normalize(cameraFront);

    for(float d = 0; d < MAX_RAY_DIST; d += RAY_STEP)
    {
        glm::vec3 pos = rayOrigin + rayDir * d;

        int x = static_cast<int>(round(pos.x));
        int y = static_cast<int>(round(pos.y));
        int z = static_cast<int>(round(pos.z));

        if(x < 0 || x >= game->getSizeX() ||
           y < 0 || y >= game->getSizeY() ||
           z < 0 || z >= game->getSizeZ())
            continue;

        if(game->isBlockSolid(x, y, z))
        {
            targetedBlock = glm::ivec3(x, y, z);
            hasTarget = true;
            return;
        }
    }
}

void Physics::breakBlock() {
    if (hasTarget && breakTimer >= BREAK_COOLDOWN)
    {
        game->setBlock(targetedBlock.x, targetedBlock.y, targetedBlock.z, 0);
        breakTimer = 0.f;
    }
}

void Physics::jump() {
    if(isGrounded)
    {
        yVelocity = JUMP_FORCE;
        isGrounded = false;
    }
}

bool Physics::checkCollision(float playerHeight, float playerRadius, glm::vec3 cameraPos)
{
    int minX = static_cast<int>(round(cameraPos.x - playerRadius));
    int maxX = static_cast<int>(round(cameraPos.x + playerRadius));
    int minY = static_cast<int>(round(cameraPos.y - playerHeight));
    int maxY = static_cast<int>(round(cameraPos.y));
    int minZ = static_cast<int>(round(cameraPos.z - playerRadius));
    int maxZ = static_cast<int>(round(cameraPos.z + playerRadius));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            for (int z = minZ; z <= maxZ; z++)
            {
                if (game->isOutOfWorld(x, y, z))
                    continue;

                if (game->isBlockSolid(x, y, z))
                    return true;
            }
        }
    }

    return false;
}

bool Physics::checkGrounded(glm::vec3 cameraPos, float playerHeight)
{
    int x = static_cast<int>(round(cameraPos.x));
    int y = static_cast<int>(floor(cameraPos.y - playerHeight));
    int z = static_cast<int>(round(cameraPos.z));

    if (game->isOutOfWorld(x, y, z))
        return false;

    return game->isBlockSolid(x, y, z);
}

float Physics::applyGravity(glm::vec3& cameraPos, float playerHeight, float deltaTime)
{
    if(checkGrounded(cameraPos, playerHeight))
    {
        isGrounded = true;
        yVelocity = 0.f;

        cameraPos.y = floor(cameraPos.y - playerHeight) + 1.f + playerHeight;
    }
    else
    {
        isGrounded = false;
    }

    if(!isGrounded)
    {
        yVelocity += GRAVITY * deltaTime;
        if(yVelocity < TERMINAL_VELOCITY)
            yVelocity = TERMINAL_VELOCITY;
    }

    return yVelocity * deltaTime;
}