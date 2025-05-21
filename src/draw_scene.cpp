#include "draw_scene.hpp"
#include <tuple>
#include "carte.hpp"


//Camera parameters
// float angle_theta{45.0}; // Angle between x axis and viewpoint
// float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{30.0};	 // Distance between origin and viewpoint

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape carre;
GLBI_Set_Of_Points frame(3);
int objectNumber = 0;




// float degToRad(float const &angle)
// {
// 	return (angle * M_PI / 2) / 90;
// }

// std::tuple<float, float, float> colorConvertor(int const &r, int const &g, int const &b)
// {
// 	return {static_cast<float>(r) / 255.0f,
// 			static_cast<float>(g) / 255.0f,
// 			static_cast<float>(b) / 255.0f};
// }

// std::tuple<float, float, float> colorConvertor(int const &color)
// {
// 	return {static_cast<float>(color) / 255.0f,
// 			static_cast<float>(color) / 255.0f,
// 			static_cast<float>(color) / 255.0f};
// }

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

}

void renderScene() {

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(Vector3D(0.0f, 0.0f, 0.0f));
	// myEngine.mvMatrixStack.addHomothety(3.0f);
	myEngine.updateMvMatrix();
    switch(objectNumber) {
    // case 0:
    //     myEngine.setFlatColor(1.0f, 0.0f, 0.0f);
    //     carre.drawShape();
    //     break;
    // default:
    //     break;
    }
}

// dessin de la grille avec des carrés
void drawSquare(float x, float y, float size) {

	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + size, y);
	glVertex2f(x + size, y + size);
	glVertex2f(x, y + size);
	glEnd();

}
void drawMap(const std::vector<std::vector<int>>& map, GLBI_Engine& myEngine) {
    int rows = map.size();
    int cols = map[0].size();

    // Taille totale de la grille en "unités OpenGL"
    // float totalWidth = 100.0f;
    float totalHeight = 100.0f;

    // Taille d'une case adaptée dynamiquement
    // float tileWidth = totalWidth / cols;
    float tileSize = totalHeight / rows;

    // On utilise la plus petite des deux pour garder des cases carrées
    // float tileSize = std::min(tileWidth, tileHeight);

    // float offsetX = -cols * tileSize / 2.0f;
    // float offsetY = -rows * tileSize / 2.0f;

    for (int yi = 0; yi < rows; ++yi) {
        for (int xi = 0; xi < cols; ++xi) {
            float x = xi / float(cols) *100.f - 50.f;
            float y = yi / float(rows) *100.f - 50.f;
            // float y = offsetY + i * tileSize;

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

	// frame.drawSet();


	// change la couleur du rectangle

	
	// ground.drawShape();

	// myEngine.mvMatrixStack.pushMatrix();
	// myEngine.mvMatrixStack.addTranslation(Vector3D(4.0f, 0.0f, 5.0f));
	// myEngine.updateMvMatrix();
	// myEngine.mvMatrixStack.addHomothety(3.0f);
	// myEngine.updateMvMatrix();
	// myEngine.setFlatColor(1.0f, 1.0f, 1.0f);
	// sphere->draw();
	// myEngine.mvMatrixStack.popMatrix();

	// myEngine.mvMatrixStack.pushMatrix();
	// myEngine.mvMatrixStack.addTranslation(Vector3D(cos(i * 0.1) * 5.0f, sin(i * 0.1) * 5.0f, 0.0f));
	// myEngine.updateMvMatrix();
	// myEngine.setFlatColor(1.0f, 1.0f, 1.0f);
	// sphere->draw();
	// // myEngine.mvMatrixStack.popMatrix();
	// i++;

	// myEngine.mvMatrixStack.pushMatrix();
	// myEngine.mvMatrixStack.addTranslation(Vector3D(0.0f, 0.0f, 10.0f));
	// myEngine.updateMvMatrix();
	// myEngine.mvMatrixStack.popMatrix();

	// drawMap(createMap());


	drawMap(map, myEngine);

}
