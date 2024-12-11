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
    glm::vec3 mPosition;
    glm::mat4 mCameraMatrix = glm::mat4(1.0f);

public:
    Camera(glm::vec3 position);

    const glm::vec3 getPosition() const;
    const glm::mat4 getCameraMatrix() const;

    // Updates and exports the camera matrix to the Vertex Shader
    virtual void updateMatrix(float FOVdeg, float nearPlane, float farPlane) = 0;
    // Send the matrix to the shader
    virtual void sendMatrix(Shader &shader, const char *uniform);
    // Handles camera inputs
    virtual void handleInputs(GLFWwindow *window) = 0;
};
