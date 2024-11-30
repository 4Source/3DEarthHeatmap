#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

// Vertices coordinates
GLfloat vertices[] = {
    // Lower left corner
    -0.5f,
    -0.5f * float(sqrt(3)) / 3,
    0.0f,

    // Lower right corner
    0.5f,
    -0.5f * float(sqrt(3)) / 3,
    0.0f,

    // Upper corner
    0.0f,
    0.5f * float(sqrt(3)) * 2 / 3,
    0.0f,

    // Inner left
    -0.5f / 2,
    0.5f * float(sqrt(3)) / 6,
    0.0f,

    // Inner right
    0.5f / 2,
    0.5f * float(sqrt(3)) / 6,
    0.0f,

    // Inner down
    0.0f,
    -0.5f * float(sqrt(3)) / 3,
    0.0f,
};

GLuint indices[] = {
    // Lower left triangle
    0,
    3,
    5,

    // Lower right triangle
    3,
    2,
    4,

    // Upper traiangle
    5,
    4,
    1,
};

int main(int argc, char const *argv[])
{
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // Specify the version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Using the core profile, that means only have the modern functions
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
    if (glewInit() != GLEW_OK)
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

    // Generate the Shader program
    Shader shaderProgram("../shader/SimpleShader.vertexshader", "../shader/SimpleShader.fragmentshader");

    // Generate the Vertex Array Object 
    VertexArrayObject vao;
    vao.Bind();

    // Generate the Vertex Buffer Object
    VertexBufferObject vbo(vertices, sizeof(vertices));
    // Generate the Element Buffer Object
    ElementBufferObject ebo(indices, sizeof(indices));

    // Link the VBO to the VAO
    vao.LinkVertexBufferObject(vbo, 0);
    // Unbind all to prevent modifying
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
        // Clear the back buffer
        glClear(GL_COLOR_BUFFER_BIT);
        // Use the shader program
        shaderProgram.Activate();
        // Bind the VAO
        vao.Bind();
        // Draw the Triangles
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        // Swap buffers
        glfwSwapBuffers(window);

        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete the objects
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
