#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath>

std::vector<std::vector<int>> createMap();
void printGrid(const std::vector<std::vector<int>> &grid);

struct Component
{
    int id;
    std::vector<std::pair<int, int>> tiles;
    int centerX;
    int centerY;
};