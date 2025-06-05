#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "carte.hpp"
#include "exe_path/exe_path.h"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "tools/shaders.hpp"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "tools/basic_mesh.hpp"
#include "glad/glad.h"
#include <iostream>
#include <tuple>

using namespace glbasimac;

// paramètres caméra
static const float Z_NEAR{0.1f};
static const float Z_FAR{500.f};
extern float angle_theta;
extern float angle_phy;
extern float dist_zoom;

/* OpenGL Engine */
extern GLBI_Engine myEngine;

void initScene();
void initTexture(GLBI_Texture &texture, const std::string &fileName);
void renderScene();
void drawScene(const std::vector<std::vector<int>> &map);
void drawSquare(float x, float y, float size, GLBI_Texture &texture);
void drawMap(const std::vector<std::vector<int>> &map, GLBI_Engine &myEngine);
void drawMenu();
void drawVictoryScreen();
void drawDefeatScreen();
extern std::vector<std::vector<int>> map;

// pour le joueur
extern float carrePosX;
extern float carrePosY;
extern int objectNumber;
extern float aspectRatio;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void restartGame();
// ennemis
float getDeltaTime();
