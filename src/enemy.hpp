#pragma once
#include "flow_field.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

struct Enemy {
    Vec2 position;
    float speed;
    Vec2 direction;    // direction courante (x, y)
    float changeDirTimer = 0.0f; // temps restant avant de changer de direction

    void update(const FlowField& flowField, const std::vector<std::vector<int>>& map, float deltaTime);
};

bool canMoveEnemy(float x, float y, float enemySize, const std::vector<std::vector<int>>& map);