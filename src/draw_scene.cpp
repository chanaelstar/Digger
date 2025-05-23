#include "draw_scene.hpp"

float dist_zoom{30.0};	 // Distance between origin and viewpoint

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape carre;
GLBI_Set_Of_Points frame(3);
int objectNumber = 0;

//Pour les sprites
//GLBI_Texture myTexture;
GLBI_Texture sandTexture;
GLBI_Texture waterTexture;
StandardMesh* rectangle;
StandardMesh* a_frame;


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

    //Application de la texture
    // rectangle = basicRect(5.0, 5.0);
    // rectangle->createVAO();
    // a_frame = createRepere(10.0);
    // a_frame->createVAO();

    // myTexture.createTexture();
    // myTexture.attachTexture();
    // myTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // int x;
    // int y;
    // int n;
    // unsigned char* data = stbi_load((exe_path::dir() / "assets/images/testMenu.jpg").string().c_str(), &x, &y, &n, 0);
    // if (!data) {
    // std::cerr << "Erreur chargement image : " << stbi_failure_reason() << std::endl;
	// }

    // myTexture.attachTexture();
	//   myTexture.loadImage(x, y, n, data);

    // stbi_image_free(data);

    // Sable
    sandTexture.createTexture();
    sandTexture.attachTexture();
    sandTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    int x, y, n;
    unsigned char* sandData = stbi_load((exe_path::dir() / "assets/images/Sand1.png").string().c_str(), &x, &y, &n, 0);
    if (!sandData) std::cerr << "Erreur chargement: " << stbi_failure_reason() << std::endl;
    sandTexture.attachTexture();
    sandTexture.loadImage(x, y, n, sandData);
    stbi_image_free(sandData);

    // Eau
    waterTexture.createTexture();
    waterTexture.attachTexture();
    waterTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    unsigned char* waterData = stbi_load((exe_path::dir() / "assets/images/Water1.png").string().c_str(), &x, &y, &n, 0);
    if (!waterData) std::cerr << "Erreur chargement water.png : " << stbi_failure_reason() << std::endl;
    waterTexture.attachTexture();
    waterTexture.loadImage(x, y, n, waterData);
    stbi_image_free(waterData);
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

    // Texture
    // myEngine.activateTexturing(true);
    // myTexture.attachTexture();

    // myEngine.setFlatColor(1.0, 1.0, 1.0);
    // rectangle->draw();

    // myTexture.detachTexture();
}
void drawTexturedTile(float x, float y, float size, GLBI_Texture& texture) {
    myEngine.activateTexturing(true);
    texture.attachTexture();

    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(x + size/2, y + size/2, 0.0f));
    myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(size/2, size/2, 1.0f));
    myEngine.updateMvMatrix();

    myEngine.setFlatColor(1.0, 1.0, 1.0);
    rectangle->draw();

    myEngine.mvMatrixStack.popMatrix();
    myEngine.updateMvMatrix();

    texture.detachTexture();
    myEngine.activateTexturing(false);
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
                
	        //myEngine.setFlatColor(1.0f, 1.0f, 1.0f); // blanc
            drawTexturedTile(x, y, tileSize, sandTexture);
            else if (val == 1)
            //myEngine.setFlatColor(0.0f, 0.0f, 0.0f); // noir
            drawTexturedTile(x, y, tileSize, waterTexture);
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





// pour les collisions (ça ne marche pas, possibilité de mettre en commentaires pour retirer les collisions)

bool collision(float x, float y, const std::vector<std::vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    float gridWidth = 100.0f;
    float tileSize = gridWidth / cols;

    int gridX = static_cast<int>((x + 50.0f) / tileSize);
    int gridY = rows - 1 - static_cast<int>((y + 50.0f) / tileSize); // y inversé

    if (gridX < 0 || gridX >= cols || gridY < 0 || gridY >= rows) return true; // mur si hors limites

    return map[gridY][gridX] == 1; // 1 = mur → collision
}


bool canMove(float newX, float newY, float size, const std::vector<std::vector<int>>& map) {
    float halfSize = size / 2.0f;

    // Coins du carré
    return collision(newX - halfSize, newY - halfSize, map) &&
           collision(newX + halfSize, newY - halfSize, map) &&
           collision(newX - halfSize, newY + halfSize, map) &&
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

