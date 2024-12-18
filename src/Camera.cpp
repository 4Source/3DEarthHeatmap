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