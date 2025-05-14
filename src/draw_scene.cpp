#include "draw_scene.hpp"
#include <tuple>
#include "carte.hpp"


/// Camera parameters
float angle_theta{45.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{30.0};	 // Distance between origin and viewpoint

GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape circle{3};

GLBI_Set_Of_Points frame(3);

IndexedMesh *sphere;
StandardMesh *cone;

float degToRad(float const &angle)
{
	return (angle * M_PI / 2) / 90;
}

std::tuple<float, float, float> colorConvertor(int const &r, int const &g, int const &b)
{
	return {static_cast<float>(r) / 255.0f,
			static_cast<float>(g) / 255.0f,
			static_cast<float>(b) / 255.0f};
}

std::tuple<float, float, float> colorConvertor(int const &color)
{
	return {static_cast<float>(color) / 255.0f,
			static_cast<float>(color) / 255.0f,
			static_cast<float>(color) / 255.0f};
}

void initScene()
{
	sphere = basicSphere();
	sphere->createVAO();

	cone = basicCone(10, 2);
	cone->createVAO();

	std::vector<float> points{0.0, 0.0, 0.0};
	somePoints.initSet(points, 1.0, 1.0, 1.0);

	std::vector<float> baseCarre{-10.0, -10.0, 0.0,
								 10.0, -10.0, 0.0,
								 10.0, 10.0, 0.0,
								 -10.0, 10.0, 0.0};
	ground.initShape(baseCarre);
	ground.changeNature(GL_TRIANGLE_FAN);

	std::vector<float> coord_point{0.0f, 0.0f, 0.0f};

	std::vector<float> coord_circle{0.0f, 0.0f, 0.0f};

	for (int i{1}; i < 100; i++)
	{
		coord_circle.push_back(sin(static_cast<float>(i) / M_PI));
		coord_circle.push_back(cos(static_cast<float>(i) / M_PI));
		coord_circle.push_back(0.0f);
	}

	circle.initShape(coord_circle);
	circle.changeNature(GL_TRIANGLE_FAN);

}

void drawMap(const std::vector<std::vector<int>>& map) {
    int rows = map.size();
    int cols = map[0].size();
    float tileSize = 1.0f; // taille d’une case

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = j * tileSize;
            float y = i * tileSize;

            int val = map[i][j];

            // Choix d'une couleur en fonction de la valeur de la case
            if (val == 0)
                glColor3f(1.0f, 1.0f, 1.0f); // blanc
            else if (val == 1)
                glColor3f(0.0f, 0.0f, 0.0f); // noir
            else
                glColor3f(1.0f, 0.0f, 0.0f); // rouge, par exemple

            // Dessin d’un carré (case)
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + tileSize, y);
            glVertex2f(x + tileSize, y + tileSize);
            glVertex2f(x, y + tileSize);
            glEnd();
        }
    }
}






int i = 0;

void drawScene()
{
	glPointSize(10.0);

	// frame.drawSet();

	myEngine.setFlatColor(0.2, 0.0, 0.0);
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

	drawMap(createMap());
}
