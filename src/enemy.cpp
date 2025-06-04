#include "enemy.hpp"
#include <cmath>
#include <iostream>


void Enemy::update(const FlowField& flowField, float deltaTime) {
    int cellX = static_cast<int>(position.x);
    int cellY = static_cast<int>(position.y);

    if (cellY < 0 || cellY >= flowField.size() || cellX < 0 || cellX >= flowField[0].size())
        return;

    Direction dir = flowField[cellY][cellX];

    Vec2 direction = { static_cast<float>(dir.dx), static_cast<float>(dir.dy) };
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0.01f) {
        direction.x /= length;
        direction.y /= length;

        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
    }

        std::cout << "Enemy position: (" << position.x << ", " << position.y << ")\n";

}
