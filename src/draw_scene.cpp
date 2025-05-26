#include "draw_scene.hpp"
#include <tuple>
#include "carte.hpp"

float dist_zoom{30.0};	 // Distance between origin and viewpoint

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape carre;
GLBI_Set_Of_Points frame(3);
int objectNumber = 0;


void initScene(){
	std::vector<float> carreCoordinates = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };
	std::vector<float> points{0.0, 0.0, 0.0};
	somePoints.initSet(points, 1.0, 1.0, 1.0);

	std::vector<float> baseCarre{-10.0, -10.0, 0.0,
								 10.0, -10.0, 0.0,
								 10.0, 10.0, 0.0,
								 -10.0, 10.0, 0.0};
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

}

void renderScene() {

    // Met à jour la projection 2D directement
    if (aspectRatio > 1.0f) {
        myEngine.set2DProjection(-50.f * aspectRatio, 50.f * aspectRatio, -50.f, 50.f);
    } else {
        myEngine.set2DProjection(-50.f, 50.f, -50.f / aspectRatio, 50.f / aspectRatio);
    }

            /* Fix camera position */

        myEngine.mvMatrixStack.loadIdentity(); 



    // joueur
    myEngine.setFlatColor(1.0f, 0.0f, 0.0f);
    carre.changeNature(GL_TRIANGLE_FAN);

    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(carrePosX, carrePosY, 0.0f));
    myEngine.updateMvMatrix();
    carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();

}

// dessin de la grille avec des carrés (pour la carte)
void drawSquare(float x, float y, float size) {

    carre.changeNature(GL_TRIANGLE_FAN);
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(x+size/2, y+size/2, 0.0f));
    myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(size/2, size/2, 0.0f));
    myEngine.updateMvMatrix();
    carre.drawShape();
    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();

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

    // Taille totale de la grille en "unités OpenGL"

    float totalHeight = 100.0f;

    // Taille d'une case adaptée dynamiquement
    float tileSize = totalHeight / rows;


    for (int yi = 0; yi < rows; ++yi) {
        for (int xi = 0; xi < cols; ++xi) {
            float x = xi / float(cols) *100.f - 50.f;
            // float y = yi / float(rows) *100.f - 50.f;
            float y = (rows - 1 - yi) / float(rows) * 100.f - 50.f; // mets la carte dans le bon sens


            int val = map[yi][xi];

            if (val == 0)

	myEngine.setFlatColor(1.0f, 1.0f, 1.0f); // blanc
            else if (val == 1)
                	myEngine.setFlatColor(0.0f, 0.0f, 0.0f); // noir
            else
                	myEngine.setFlatColor(1.0f, 0.0f, 0.0f); // rouge

            drawSquare(x, y, tileSize);
        }
    }
}


int i = 0;

void drawScene(const std::vector<std::vector<int>>& map)
{
	glPointSize(10.0);
	drawMap(map, myEngine);
}

// pour le joueur 


/* Position du carré */
float carrePosX = 0.0f;
float carrePosY = 0.0f;

/* Moteur graphique et objets */
GLBI_Set_Of_Points thePoints;

float aspectRatio = 1.0f; // ou une autre valeur par défaut


extern std::vector<std::vector<int>> map; // à rendre globale si besoin





// pour les collisions 
// pour les collisions (ça ne marche pas, possibilité de mettre en commentaires pour retirer les collisions)

bool collision(float x, float y, const std::vector<std::vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    float gridWidth = 100.0f;
    float tileSize = gridWidth / cols;

    // Convertit la position en coordonnées de la grille
    int gridX = static_cast<int>((x + 50.0f) / tileSize);
    //int gridY = static_cast<int>((y + 50.0f) / tileSize);
    int gridY = rows - 1 - static_cast<int>((y + 50.0f) / tileSize); // y inversé

    // Vérifie les limites
    if (gridX < 0 || gridX >= cols || gridY < 0 || gridY >= rows) return false;
    if (gridX < 0 || gridX >= cols || gridY < 0 || gridY >= rows) return true; // mur si hors limites

    // 0 = sol, 1 = mur
    return map[gridY][gridX] == 0;
    return map[gridY][gridX] == 1; // 1 = mur → collision
}


bool canMove(float newX, float newY, float size, const std::vector<std::vector<int>>& map) {
    float halfSize = size / 2.0f;
           collision(newX + halfSize, newY + halfSize, map);
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        float newX = carrePosX;
        float newY = carrePosY;
        float playerSize = 2.0f; // taille du carré joueur

        switch (key)
        {
        case GLFW_KEY_W:
            newY += 0.1f;
            break;
        case GLFW_KEY_S:
            newY -= 0.1f;
            break;
        case GLFW_KEY_A:
            newX -= 0.1f;
            break;
        case GLFW_KEY_D:
            newX += 0.1f;
            break;
        default:
            break;
        }

        // Collision
        if (canMove(newX, newY, playerSize, map)) {
            carrePosX = newX;
            carrePosY = newY;
        }
    }
}