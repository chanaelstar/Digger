#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR {0.1f};
static const float Z_FAR {500.f};
extern float angle_theta;      // Angle between x axis and viewpoint
extern float angle_phy  ;      // Angle between z axis and viewpoint
extern float dist_zoom  ;      // Distance between origin and viewpoint

/* OpenGL Engine */
extern GLBI_Engine myEngine;

void initScene();
void renderScene();
void drawScene(const std::vector<std::vector<int>>& map);
void drawSquare(float x, float y, float size);
void drawMap(const std::vector<std::vector<int>>& map);

