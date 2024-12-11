#pragma once

#include "Camera.h"

class FreeCamera : public Camera
{
protected:
    glm::vec3 mOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);

    int mWidth, mHeight;

    float mSpeed = 0.1f;
    float mSensitivity = 50.0f;

public:
    FreeCamera(int width, int height, glm::vec3 position);

    virtual void updateMatrix(float FOVdeg, float nearPlane, float farPlane) override;
    virtual void handleInputs(GLFWwindow *window) override;
};
