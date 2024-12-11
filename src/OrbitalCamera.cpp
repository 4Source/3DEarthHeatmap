#include "OrbitalCamera.h"

OrbitalCamera::OrbitalCamera(int width, int height, glm::vec3 position, glm::vec3 targetPosition) : Camera(position)
{
    // TODO: Calculate the Up and Right Vector
    mUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
    mRightVec = glm::vec3(1.0f, 0.0f, 0.0f);
    OrbitalCamera::mWidth = width;
    OrbitalCamera::mHeight = height;
    OrbitalCamera::mTargetPosition = targetPosition;
}

void OrbitalCamera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(mPosition, mTargetPosition, mUpVec);

    // Apply the perspective
    proj = glm::perspective(glm::radians(FOVdeg), (float)(mWidth / mHeight), nearPlane, farPlane);

    mCameraMatrix = proj * view;
}

void OrbitalCamera::handleInputs(GLFWwindow *window)
{
    float delta = mSpeed / mSensitivity;
    glm::vec3 rotationAxis(0.0f);

    // Collect active inputs and their associated rotation axes
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        // Rotate around the right vector
        rotationAxis -= mRightVec;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // Rotate around the up vector
        rotationAxis -= mUpVec;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // Rotate around the right vector
        rotationAxis += mRightVec;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // Rotate around the up vector
        rotationAxis += mUpVec;
    }

    if (glm::length(rotationAxis) > 0.0f)
    {
        // Normalize the rotation axis to avoid faster rotations when multiple keys are pressed
        rotationAxis = glm::normalize(rotationAxis);

        // Apply rotation
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(delta * 90), rotationAxis);
        mPosition = glm::vec3(rot * glm::vec4(mPosition, 1.0f));
        mUpVec = glm::vec3(rot * glm::vec4(mUpVec, 1.0f));
        mRightVec = glm::vec3(rot * glm::vec4(mRightVec, 1.0f));
    }

    // Move up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // Direction the camera is looking minus the current position gives the direction vector
        mPosition -= glm::normalize(mTargetPosition - mPosition) * delta;
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        // Direction the camera is looking minus the current position gives the direction vector
        mPosition += glm::normalize(mTargetPosition - mPosition) * delta;
    }
}