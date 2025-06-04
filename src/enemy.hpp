#pragma once
#include "flow_field.hpp"

struct Enemy {
    Vec2 position;
    float speed;

    void update(const FlowField& flowField, float deltaTime);
};
