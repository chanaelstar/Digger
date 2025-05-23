#pragma once
#define GLFW_INCLUDE_NONE
#include <tuple>
#include "carte.hpp"
#include "exe_path/exe_path.h"
#include "glbasimac/glbi_texture.hpp"
#include "tools/shaders.hpp"
#include "tools/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <iostream>


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
void drawTexturedTile(float x, float y, float size, GLBI_Texture& texture);

extern std::vector<std::vector<int>> map;



// pour le joueur

extern float carrePosX;
extern float carrePosY;
extern int objectNumber;
extern float aspectRatio;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



