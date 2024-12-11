#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

#include "Shader.h"

class Camera
{
protected:
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    int width, height;

    float speed = 0.1f;
    float sensitivity = 100.0f;

public:
    glm::vec3 position;
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    Camera(int width, int height, glm::vec3 position);

    // Updates and exports the camera matrix to the Vertex Shader
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(Shader &shader, const char *uniform);
    // Handles camera inputs
    void Inputs(GLFWwindow *window);
};
