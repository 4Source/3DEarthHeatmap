#include "Camera.h"

Camera::Camera(glm::vec3 position)
{
    Camera::mPosition = position;
}

const glm::vec3 Camera::getPosition() const
{
    return mPosition;
}

const glm::mat4 Camera::getCameraMatrix() const
{
    return mCameraMatrix;
}

void Camera::sendMatrix(Shader &shader, const char *uniform)
{
    // Input projection matix to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), uniform), 1, GL_FALSE, glm::value_ptr(mCameraMatrix));
}