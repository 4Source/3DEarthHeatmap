#pragma once

#include "Camera.h"

class OrbitalCamera : public Camera
{
protected:
    glm::vec3 mTargetPosition;
    glm::vec3 mUpVec;
    glm::vec3 mRightVec;

    int mWidth, mHeight;

    float mSpeed = 0.1f;
    float mSensitivity = 30.0f;

public:
    OrbitalCamera(int width, int height, glm::vec3 position, glm::vec3 targetPosition);

    virtual void updateMatrix(float FOVdeg, float nearPlane, float farPlane) override;
    virtual void handleInputs(GLFWwindow *window) override;
};
