#pragma once
#include <vector>

class World
{
public:
    World(int sizeX = 64, int sizeY = 8, int sizeZ = 64);

    const int WORLD_X;
    const int WORLD_Y;
    const int WORLD_Z;

    bool isBlockSolid(int x, int y, int z) const;
    void setBlock(int x, int y, int z, int value);
    bool isOutOfWorld(int x, int y, int z) const;

private:
    int getIndex(int x, int y, int z) const;

    std::vector<int> blocks;
};