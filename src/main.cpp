#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include <iostream>
#include <cmath>
#include "draw_scene.hpp"
#include "carte.hpp"
#include <vector>
#include "enemy.hpp"
#include "flow_field.hpp"


float lastTime = 0.0f;



// il n'aime pas ça 
std::vector<std::vector<int>> map = createMap();
using namespace glbasimac;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
// static float aspectRatio = 1.0f;

/* Window size */
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;

/* Espace virtuel */
static const float GL_VIEW_SIZE = 10.0;

// Menu
enum class AppState {
    MENU,
    JEU,
    QUIT
};

AppState currentState = AppState::MENU;

/* Error handling function */
void onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}


void onWindowResized(GLFWwindow * /*window*/, int width, int height) {
     aspectRatio = width / (float)height;
     glViewport(0, 0, width, height);

     if(aspectRatio > 1.0) {
         myEngine.set2DProjection(-GL_VIEW_SIZE * aspectRatio / 2., GL_VIEW_SIZE * aspectRatio / 2., -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
     } else {
         myEngine.set2DProjection(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2., -GL_VIEW_SIZE / (2. * aspectRatio), GL_VIEW_SIZE / (2. * aspectRatio));
     }
 }

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convertir en coordonnées OpenGL
        float x = (xpos / width) * GL_VIEW_SIZE - GL_VIEW_SIZE / 2.f;
        float y = ((height - ypos) / height) * GL_VIEW_SIZE - GL_VIEW_SIZE / 2.f;

        if (currentState == AppState::MENU) {
            if (x > -3 && x < 3 && y > 1 && y < 3) {
                currentState = AppState::JEU; 
            } else if (x > -3 && x < 3 && y > -3 && y < -1) {
                currentState = AppState::QUIT;
            }
        }
    }
}



// void renderScene() {
//     float currentTime = glfwGetTime();
//     float deltaTime = currentTime - lastTime;
//     lastTime = currentTime;

//     // Optionnel : Recalculer le flow field si le joueur a bougé d'une cellule
//     static int prevPlayerX = -1;
//     static int prevPlayerY = -1;
//     int playerX = static_cast<int>(carrePosX);
//     int playerY = static_cast<int>(carrePosY);
//     if (playerX != prevPlayerX || playerY != prevPlayerY) {
//         flowField = computeFlowField(map, playerX, playerY);
//         prevPlayerX = playerX;
//         prevPlayerY = playerY;
//     }

//     // Met à jour la position des ennemis
//     for (auto& enemy : enemies) {
//         enemy.update(flowField, deltaTime);
//     }

//     // Dessin de la carte
//     drawMap(map, myEngine);

//     // Dessin des ennemis
//     for (const auto& enemy : enemies) {
//         drawSquare(enemy.position.x, enemy.position.y, 1.0f);
//     }
// }


int main()
{
    const double FRAMERATE_IN_SECONDS = 1. / 30.;
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    /* Callback to a function if an error is rised by GLFW */
    glfwSetErrorCallback(onError);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Digger", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // -- Callbacks --
    glfwSetWindowSizeCallback(window, onWindowResized);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Intialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    // Initialize Rendering Engine
    myEngine.initGL();

    onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    // glfwSetKeyCallback(window, key_callback);

    // Initaialize the set of points
    initScene();

    // pour le player
    objectNumber = 0;

    // auto map = createMap();
    // std::vector<std::vector<int>> map;

    printGrid(map);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Get time (in second) at loop beginning */
        double startTime = glfwGetTime();

        if (currentState == AppState::MENU) {
            drawMenu();
        }
        
        else if (currentState == AppState::JEU) {
            glClearColor(0.2f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            renderScene();
            drawScene(map);
        }

        else if (currentState == AppState::QUIT) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();

        /* Elapsed time computation from loop begining */
        double elapsedTime = glfwGetTime() - startTime;
        /* If to few time is spend vs our wanted FPS, we wait */
        while (elapsedTime < FRAMERATE_IN_SECONDS)
        {
            glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
            elapsedTime = glfwGetTime() - startTime;
        }
    }

    glfwTerminate();
    return 0;
}