#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"
#include "draw_scene.hpp"

float dist_zoom{30.0};	 // Distance between origin and viewpoint

/* Position du carré */
float carrePosX = 0.0f;
float carrePosY = 0.0f;

float aspectRatio = 1.0f; // ou une autre valeur par défaut

/* Espace virtuel */
static const float GL_VIEW_SIZE = 10.0;

extern std::vector<std::vector<int>> map;

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape carre;
GLBI_Set_Of_Points thePoints;
GLBI_Set_Of_Points frame(3);
int objectNumber = 0;

struct Vector2D {
    float x;
    float y;
};

float playerX = 1.5f; // Position initiale du joueur en indices de grille
float playerY = 1.5f;

// Convertit des indices de grille en coordonnées OpenGL (centre de la case)
Vector2D gridToWorld(int gridX, int gridY, int rows, int cols) {
    float x = gridX + 0.5f;
    float y = rows - 1 - gridY + 0.5f;
    return {x, y};
}

// Convertit une position OpenGL (x, y) en indices de grille
std::pair<int, int> worldToGrid(float x, float y, int rows, int cols) {
    int gridX = int(x);
    int gridY = rows - 1 - int(y);
    return {gridX, gridY};
}

// Initialisation de la position du joueur
void initPlayerPosition() {
    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[0].size(); ++x) {
            if (map[y][x] == 0) { // case blanche
                playerX = x + 0.5f;
                playerY = map.size() - 1 - y + 0.5f;
                return;
            }
        }
    }
}
void initScene(){
	std::vector<float> carreCoordinates = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };
	std::vector<float> points{0.0, 0.0, 0.0};
	somePoints.initSet(points, 1.0, 1.0, 1.0);

	std::vector<float> baseCarre{
        -10.0, -10.0, 0.0,
	    10.0, -10.0, 0.0,
		10.0, 10.0, 0.0,
		-10.0, 10.0, 0.0
    };
	carre.initShape(carreCoordinates);
	ground.initShape(baseCarre);
	carre.changeNature(GL_TRIANGLE_FAN);
	ground.changeNature(GL_TRIANGLE_FAN);

	std::vector<float> playerCoordinates = {
        -1.f, -1.f,
         1.f, -1.f,
         1.f,  1.f,
        -1.f,  1.f
    };
    carre.initShape(playerCoordinates);
    initPlayerPosition();
}



void drawMenu() {
    glClearColor(0.1f, 0.1f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Bouton Demarrer
    myEngine.setFlatColor(0.2f, 0.8f, 0.2f); // Vert
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(0.0f, 2.0f, 0.0f));
    myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(3.0f, 1.0f, 1.0f));
    myEngine.updateMvMatrix();
    carre.changeNature(GL_TRIANGLE_FAN);
    carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();

    // Bouton Quitter
    myEngine.setFlatColor(0.8f, 0.2f, 0.2f); // Rouge
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(0.0f, -2.0f, 0.0f));
    myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(3.0f, 1.0f, 1.0f));
    myEngine.updateMvMatrix();
    carre.changeNature(GL_TRIANGLE_FAN);
    carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();
}



void drawMap(const std::vector<std::vector<int>>& map, GLBI_Engine& myEngine) {
    int rows = map.size();
    int cols = map[0].size();
    for (int yi = 0; yi < rows; ++yi) {
        for (int xi = 0; xi < cols; ++xi) {
            float x = xi + 0.5f;
            float y = rows - 1 - yi + 0.5f;

            int val = map[yi][xi];

            if (val == 0) {
                myEngine.setFlatColor(1.0f, 1.0f, 1.0f); // blanc
                drawSquare(x, y, 1.0f);
            }
            else if (val == 1) {
                myEngine.setFlatColor(0.0f, 0.0f, 0.0f); // noir
                drawSquare(x, y, 1.0f);
            }
            else if (val == 2) {
                myEngine.setFlatColor(1.0f, 1.0f, 0.0f); // jaune (les blocs minables)
                drawSquare(x, y, 1.0f);
            }
            else if (val == 3) {
                myEngine.setFlatColor(0.0f, 0.0f, 1.0f); // bleu (les récompenses)
                drawSquare(x, y, 1.0f);
            }
            else {
                myEngine.setFlatColor(1.0f, 0.0f, 0.0f); // rouge
                drawSquare(x, y, 1.0f);
            }
        }
    }
}

void renderScene() {
    int rows = map.size();
    int cols = map[0].size();
    float playerSize = 0.95f; 

    // Affichage plein écran
    myEngine.set2DProjection(0.f, float(cols), 0.f, float(rows));

    myEngine.mvMatrixStack.loadIdentity();

    // joueur
    myEngine.setFlatColor(1.0f, 0.0f, 0.0f);
    carre.changeNature(GL_TRIANGLE_FAN);

    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(playerX, playerY, 0.0f));
    myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(playerSize/2, playerSize/2, 1.0f));
    myEngine.updateMvMatrix();
    carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();
}

// dessin de la grille avec des carrés (pour la carte)
void drawSquare(float x, float y, float size) {
    carre.changeNature(GL_TRIANGLE_FAN);
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(x, y, 0.0f));
    myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(size/2, size/2, 0.0f));
    myEngine.updateMvMatrix();
    carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();
}


int i = 0;

void drawScene(const std::vector<std::vector<int>>& map)
{
	glPointSize(10.0);
	drawMap(map, myEngine);
}

// pour le joueur
// pour les collisions (ça ne marche pas, possibilité de mettre en commentaires pour retirer les collisions)

bool canMoveSquare(float x, float y, float playerSize, const std::vector<std::vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    float half = playerSize / 2.0f;

    // On teste les 4 coins du joueur
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



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        float step = 0.15f;
        float newX = playerX;
        float newY = playerY;
        float playerSize = 0.95f;

        switch (key)
        {
        case GLFW_KEY_UP:
            newY += step;
            break;
        case GLFW_KEY_DOWN:
            newY -= step;
            break;
        case GLFW_KEY_LEFT:
            newX -= step;
            break;
        case GLFW_KEY_RIGHT:
            newX += step;
            break;
        default:
            break;
        }
        if (canMoveSquare(newX, newY, playerSize, map))
        {
            playerX = newX;
            playerY = newY;

            // Vérification des collisions avec les blocs minables
            auto [gridX, gridY] = worldToGrid(playerX, playerY, map.size(), map[0].size());

            if (gridX >= 0 && gridX < map[0].size() && gridY >= 0 && gridY < map.size()) {
                if (map[gridY][gridX] == 2 || map[gridY][gridX] == 3) {
                    map[gridY][gridX] = 0; // Le bloc devient vide
                }
            }
        }
    }
}