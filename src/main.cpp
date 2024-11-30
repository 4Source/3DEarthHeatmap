#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Texture.h"

// Vertices coordinates
GLfloat vertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
        0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f};

// Indices for vertices order
GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4};

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
    const unsigned int width = 1536, height = 1536;
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

    // Link the attributes to the VAO
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);                   // Vertex position
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Vertex color
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float))); // Vertex Texture coordinate
    // Unbind all to prevent modifying
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // Texture
    Texture tex0("../assets/textures/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    tex0.textureUnit(shaderProgram, "tex0", 0);

    // Unbind the OpenGL texture object
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create a uniform for texture
    GLuint uniTex0ID = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(uniTex0ID, 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    // Enable Detph testing
    glEnable(GL_DEPTH_TEST);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
        // Clear the back buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Use the shader program
        shaderProgram.Activate();

        // Update rotation of model
        double currTime = glfwGetTime();
        if (currTime - prevTime >= 1.0f / 60)
        {
            rotation += 0.5f;
            prevTime = currTime;
        }

        // Model-View-Projection Matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // Apply rotation to the model
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        // Input model matix to shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Apply the view transformation
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        // Input view matix to shader
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Apply the perspective
        proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
        // Input projection matix to shader
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Bind the Texture
        tex0.Bind();
        // Bind the VAO
        vao.Bind();
        // Draw the Triangles
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        // Swap buffers
        glfwSwapBuffers(window);

        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete the objects
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    tex0.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
