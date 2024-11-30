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

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
