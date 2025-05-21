// Synthèse d'image
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
// Algo
#include <iostream>
#include <cmath>
#include "draw_scene.hpp"
#include "carte.hpp"

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



// void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
// {
//     if (action == GLFW_PRESS || action == GLFW_REPEAT)
//     {
//         switch (key)
//         {
//         case GLFW_KEY_ESCAPE:
//             glfwSetWindowShouldClose(window, GL_TRUE);
//             break;
//         case GLFW_KEY_W:
//             angle_phy += 1.0f;
//             break;
//         case GLFW_KEY_S:
//             angle_phy -= 1.0f;
//             break;
//         case GLFW_KEY_A:
//             angle_theta -= 1.0f;
//             break;
//         case GLFW_KEY_D:
//             angle_theta += 1.0f;
//             break;
//         case GLFW_KEY_UP:
//             dist_zoom *=0.9f;
//             break;
//         case GLFW_KEY_DOWN:
//             dist_zoom *= 1.1f;
//             break;
//         default:
//             break;
//         }
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

    auto map = createMap();
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

    

        /* Fix camera position */
        // myEngine.mvMatrixStack.loadIdentity();


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