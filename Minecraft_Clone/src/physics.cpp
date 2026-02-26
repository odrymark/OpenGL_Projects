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

        if(game->isOutOfWorld(x, y, z))
        {
            isBlockPlaceable = false;
            continue;
        }

        if(game->isBlockSolid(x, y, z))
        {
            targetedBlock = glm::ivec3(x, y, z);
            hasTarget = true;
            return;
        }

        isBlockPlaceable = true;
        prevAirBlock = glm::ivec3(x, y, z);
    }
}

void Physics::breakBlock()
{
    if (hasTarget && breakTimer >= BREAK_COOLDOWN)
    {
        game->setBlock(targetedBlock.x, targetedBlock.y, targetedBlock.z, 0);
        breakTimer = 0.f;
    }
}

void Physics::placeBlock(glm::vec3 cameraPos, float playerHeight, float playerRadius)
{
    int minX = static_cast<int>(round(cameraPos.x - playerRadius));
    int maxX = static_cast<int>(round(cameraPos.x + playerRadius));
    int minY = static_cast<int>(round(cameraPos.y - playerHeight));
    int maxY = static_cast<int>(round(cameraPos.y));
    int minZ = static_cast<int>(round(cameraPos.z - playerRadius));
    int maxZ = static_cast<int>(round(cameraPos.z + playerRadius));

    if(hasTarget && isBlockPlaceable && placeTimer >= PLACE_COOLDOWN &&
        glm::ivec3(minX, minY, minZ) != prevAirBlock &&
        glm::ivec3(minX, minY, maxZ) != prevAirBlock &&
        glm::ivec3(maxX, minY, minZ) != prevAirBlock &&
        glm::ivec3(maxX, minY, maxZ) != prevAirBlock &&
        glm::ivec3(minX, maxY, minZ) != prevAirBlock &&
        glm::ivec3(minX, maxY, maxZ) != prevAirBlock &&
        glm::ivec3(maxX, maxY, minZ) != prevAirBlock &&
        glm::ivec3(maxX, maxY, maxZ) != prevAirBlock)
    {
        game->setBlock(prevAirBlock.x, prevAirBlock.y, prevAirBlock.z, 1);
        placeTimer = 0.f;
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

bool Physics::checkGrounded(glm::vec3 cameraPos, float playerRadius, float playerHeight)
{
    int xMin = static_cast<int>(round(cameraPos.x - playerRadius));
    int xMax = static_cast<int>(round(cameraPos.x + playerRadius));
    int y = static_cast<int>(floor(cameraPos.y - playerHeight));
    int zMin = static_cast<int>(round(cameraPos.z - playerRadius));
    int zMax = static_cast<int>(round(cameraPos.z + playerRadius));

    if (game->isOutOfWorld(xMax, y, zMax))
        return false;

    if(game->isBlockSolid(xMin, y, zMin) || game->isBlockSolid(xMax, y, zMax) || game->isBlockSolid(xMax, y, zMin) || game->isBlockSolid(xMin, y, zMax))
        return true;

    return false;
}

float Physics::applyGravity(glm::vec3& cameraPos, float playerRadius, float playerHeight, float deltaTime)
{
    if(checkGrounded(cameraPos, playerRadius, playerHeight))
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