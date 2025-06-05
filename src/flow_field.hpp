#pragma once
#include <vector>
#include <queue>
#include <utility>
#include <cmath>

struct Vec2
{
    float x, y;
};

struct Direction
{
    int dx, dy;
};

// FlowField stocke pour chaque case un vecteur de direction
using FlowField = std::vector<std::vector<Direction>>;

FlowField computeFlowField(const std::vector<std::vector<int>> &map, int targetX, int targetY);

// test
// extern std::vector<Enemy> enemies;
extern FlowField flowField;
extern std::vector<std::vector<int>> map;
