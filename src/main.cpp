#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
// Fragment Shader source code
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
                                   "}\n\0";

int main(int argc, char const *argv[])
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    // Specify the version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Using the core profile, that means only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Vertices coordinates
    GLfloat vertices[] = {
        -0.5f,
        -0.5f * float(sqrt(3)) / 3,
        0.0f,

        0.5f,
        -0.5f * float(sqrt(3)) / 3,
        0.0f,

        0.0f,
        0.5f * float(sqrt(3)) * 2 / 3,
        0.0f,
    };

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

    // Create a Vertex Shader Object
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile the vertex shader
    glCompileShader(vertexShader);

    // Create a Fragment Shader Object
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile the fragment shader
    glCompileShader(fragmentShader);

    // Create Shader Program Object
    GLuint shaderProgram = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Link all the shaders together into the Shader Program
    glLinkProgram(shaderProgram);

    // Delete the Vertex and Fragment Shader object because there are allready in the Shader Program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generate the Vertex Array Object and the Vertex Buffer Object with only 1 object each
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Make it to the current Vertex Array Object
    glBindVertexArray(VAO);

    // Bind the VBO specifying it is a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Add the vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // Enable the Vertex Attribute
    glEnableVertexAttribArray(0);

    // Bind both to 0 so that don't accidentally modify them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Specify the color of the background
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    // Clear the back buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap buffers
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
        // Clear the back buffer
        glClear(GL_COLOR_BUFFER_BIT);
        // Use the shader program
        glUseProgram(shaderProgram);
        // Bind the VAO
        glBindVertexArray(VAO);
        // Draw the Triangles
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Swap buffers
        glfwSwapBuffers(window);

        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete the objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
