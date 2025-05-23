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
static const float GL_VIEW_SIZE = 100.0;

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

         // Efface l'écran (à faire avant tout dessin)
        glClearColor(0.2f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        /* Render here */
        renderScene();
        drawScene(map); 

    



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