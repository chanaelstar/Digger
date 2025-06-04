#include "enemy.hpp"
#include <cmath>
#include <iostream>

std::pair<int, int> worldToGrid(float x, float y, int rows, int cols);
// Convertit une position OpenGL (x, y) en indices de grille
std::pair<int, int> worldToGrid(float x, float y, int rows, int cols);

bool canMoveEnemy(float x, float y, float enemySize, const std::vector<std::vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    float half = enemySize / 2.0f;

    // On teste les 4 coins de l'ennemi
    std::pair<int, int> corners[4] = {
        worldToGrid(x - half, y - half, rows, cols),
        worldToGrid(x + half, y - half, rows, cols),
        worldToGrid(x - half, y + half, rows, cols),
        worldToGrid(x + half, y + half, rows, cols)
    };

    for (int i = 0; i < 4; ++i) {
        int gridX = corners[i].first;
        int gridY = corners[i].second;
        if (gridX < 0 || gridX >= cols || gridY < 0 || gridY >= rows)
            return false;
        if (map[gridY][gridX] == 1 )
            return false;
    }
    return true;
}

void Enemy::update(const FlowField& /*flowField*/, const std::vector<std::vector<int>>& map, float deltaTime, const Vec2& playerPos) {
    float enemySize = 0.8f;

    // Diminue le timer
    changeDirTimer -= deltaTime;

    // Calcul de la distance en cases entre l'ennemi et le joueur
    int rows = map.size();
    int cols = map[0].size();
    auto [enemyGridX, enemyGridY] = worldToGrid(position.x, position.y, rows, cols);
    auto [playerGridX, playerGridY] = worldToGrid(playerPos.x, playerPos.y, rows, cols);
    int dist = std::abs(enemyGridX - playerGridX) + std::abs(enemyGridY - playerGridY);

    if (dist <= 5) {
        // Poursuite du joueur : direction vers le joueur
        float dx = playerPos.x - position.x;
        float dy = playerPos.y - position.y;
        float len = std::sqrt(dx*dx + dy*dy);
        if (len > 0.01f) {
            direction.x = dx / len;
            direction.y = dy / len;
        }
        // Pas de changement de direction aléatoire
    } else {
        // Si timer écoulé ou direction nulle, choisis une nouvelle direction aléatoire
        if (changeDirTimer <= 0.0f || (direction.x == 0 && direction.y == 0)) {
            int dirs[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
            int idx = rand() % 4;
            direction.x = static_cast<float>(dirs[idx][0]);
            direction.y = static_cast<float>(dirs[idx][1]);
            changeDirTimer = 1.0f + (rand() % 100) / 100.0f; // entre 1 et 2 secondes
        }
    }

    // Tente d'avancer dans la direction courante
    float newX = position.x + direction.x * speed * deltaTime;
    float newY = position.y + direction.y * speed * deltaTime;

    if (canMoveEnemy(newX, newY, enemySize, map)) {
        position.x = newX;
        position.y = newY;
    } else {
        // Si bloqué, force un changement de direction au prochain update
        changeDirTimer = 0.0f;
    }
}
