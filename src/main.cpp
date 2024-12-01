#include <iostream>

#include "Model.h"

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

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Enable Detph testing
    glEnable(GL_DEPTH_TEST);

    // Create camera
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // Load model from file
    Model model("../assets/models/sword/scene.gltf");

    double prevTime = glfwGetTime();
    double frameTime = 1.0 / 144;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
        // Clear the back buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle camera inputs
        camera.Inputs(window);
        // Update camera matrix
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        model.Draw(shaderProgram, camera);

        // Swap buffers
        glfwSwapBuffers(window);

        // Take care of all GLFW events
        glfwPollEvents();

        // Constant frame time
        double currTime = glfwGetTime();
        while (currTime - prevTime < frameTime)
        {
            currTime = glfwGetTime();
        }
        if (currTime - prevTime > frameTime)
        {
            prevTime = currTime;
        }
    }

    // Delete the objects
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
