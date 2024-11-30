#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


int main(int argc, char const *argv[])
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    int width = 1536, height = 1536;
    GLFWwindow *window = glfwCreateWindow(width, height, "3D Earth Heatmap", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if(glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set the viewport to the full window
    glViewport(0, 0, width, height);

    // Specify the color of the background
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    // Clear the back buffer 
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap buffers
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
