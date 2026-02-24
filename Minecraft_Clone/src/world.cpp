#include "world.h"
#include <stdexcept>

World::World(int sizeX, int sizeY, int sizeZ)
    : WORLD_X(sizeX), WORLD_Y(sizeY), WORLD_Z(sizeZ)
{
    blocks.resize(WORLD_X * WORLD_Y * WORLD_Z, 0);

    for (int x = 0; x < WORLD_X; x++)
        for (int y = 0; y < WORLD_Y; y++)
            for (int z = 0; z < WORLD_Z; z++)
                setBlock(x, y, z, 1);
}

int World::getIndex(int x, int y, int z) const
{
    if (x < 0 || x >= WORLD_X ||
        y < 0 || y >= WORLD_Y ||
        z < 0 || z >= WORLD_Z)
        throw std::out_of_range("Block coordinates out of bounds");

    return x + y * WORLD_X + z * WORLD_X * WORLD_Y;
}

bool World::isBlockSolid(int x, int y, int z) const
{
    return blocks[getIndex(x, y, z)] != 0;
}

void World::setBlock(int x, int y, int z, int value)
{
    blocks[getIndex(x, y, z)] = value;
}

bool World::isOutOfWorld(int x, int y, int z) const
{
    return x < 0 || x >= WORLD_X ||
           y < 0 || y >= WORLD_Y ||
           z < 0 || z >= WORLD_Z;
}