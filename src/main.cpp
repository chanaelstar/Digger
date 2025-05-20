#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "draw_scene.hpp"
#include "tools/matrix_stack.hpp"
#include <iostream>
#include <cmath>
#include "carte.hpp"
 #include "glbasimac/glbi_engine.hpp"
 #include "glbasimac/glbi_set_of_points.hpp"
 #include "glbasimac/glbi_convex_2D_shape.hpp"

/* Window size */
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;

using namespace glbasimac;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;
static float aspectRatio = 1.0f;

 /* Espace virtuel */
 static const float GL_VIEW_SIZE = 100.0;

/* Error handling function */
void onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

// void onWindowResized(GLFWwindow *, int width, int height)
// {
//     aspectRatio = width / (float)height;
//     myEngine.set2DProjection(0.0f, WINDOW_WIDTH / 100.0f, 0.0f, WINDOW_HEIGHT / 100.0f);
// };
 void onWindowResized(GLFWwindow * /*window*/, int width, int height) {
     aspectRatio = width / (float)height;
     glViewport(0, 0, width, height);
 
     if(aspectRatio > 1.0) {
         myEngine.set2DProjection(-GL_VIEW_SIZE * aspectRatio / 2., GL_VIEW_SIZE * aspectRatio / 2., -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
     } else {
         myEngine.set2DProjection(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2., -GL_VIEW_SIZE / (2. * aspectRatio), GL_VIEW_SIZE / (2. * aspectRatio));
     }
 }


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_W:
            angle_phy += 1.0f;
            break;
        case GLFW_KEY_S:
            angle_phy -= 1.0f;
            break;
        case GLFW_KEY_A:
            angle_theta -= 1.0f;
            break;
        case GLFW_KEY_D:
            angle_theta += 1.0f;
            break;
        case GLFW_KEY_UP:
            dist_zoom *=0.9f;
            break;
        case GLFW_KEY_DOWN:
            dist_zoom *= 1.1f;
            break;
        default:
            break;
        }
    }
}

int main()
{
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

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Intialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    // Initialize Rendering Engine
    myEngine.mode2D = true;
    myEngine.initGL();

    onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetKeyCallback(window, key_callback);

    initScene();

    // auto const map = createMap();
    // static std::vector<std::vector<int>> map = createMap();
    // drawScene(createMap());
    // drawScene(map);

    auto map = createMap();  // ou std::vector<std::vector<int>> map = ...
    drawScene(map); 

    printGrid(createMap());

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Get time (in second) at loop beginning */
        double startTime = glfwGetTime();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        /* Fix camera position */
        myEngine.mvMatrixStack.loadIdentity();
        // Vector3D pos_camera = Vector3D(dist_zoom * cos(deg2rad(angle_theta)) * cos(deg2rad(angle_phy)),
        //                                dist_zoom * sin(deg2rad(angle_theta)) * cos(deg2rad(angle_phy)),
        //                                dist_zoom * sin(deg2rad(angle_phy)));

        // Vector3D viewed_point = Vector3D(0.0f, 0.0f, 0.0f);
        // Vector3D up_vector = Vector3D(0.0f, 0.0f, 1.0f);
        // Matrix4D view_matrix = Matrix4D::lookAt(pos_camera, viewed_point, up_vector);
        // myEngine.setViewMatrix(view_matrix);
        // myEngine.updateMvMatrix();

        // drawScene(map);

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