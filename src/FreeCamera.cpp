#include "FreeCamera.h"

FreeCamera::FreeCamera(int width, int height, glm::vec3 position) : Camera(position)
{
    FreeCamera::mWidth = width;
    FreeCamera::mHeight = height;
}

void FreeCamera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(mPosition, mPosition + mOrientation, mUp);

    // Apply the perspective
    proj = glm::perspective(glm::radians(FOVdeg), (float)(mWidth / mHeight), nearPlane, farPlane);

    mCameraMatrix = proj * view;
}

void FreeCamera::handleInputs(GLFWwindow *window)
{
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mPosition += (mSpeed / mSensitivity) * mOrientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mPosition += (mSpeed / mSensitivity) * -glm::normalize(glm::cross(mOrientation, mUp));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        mPosition += (mSpeed / mSensitivity) * -mOrientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mPosition += (mSpeed / mSensitivity) * glm::normalize(glm::cross(mOrientation, mUp));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        mPosition += (mSpeed / mSensitivity) * mUp;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        mPosition += (mSpeed / mSensitivity) * -mUp;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        mSpeed = 0.4f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        mSpeed = 0.1f;
    }
}
