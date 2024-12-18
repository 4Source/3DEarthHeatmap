#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"
#include "Model.h"
#include "Camera.h"

class Shader
{
private:
    GLuint mId;

public:
    // Contructor that build the Shaders from file
    Shader(const char *vertexFile, const char *fragmentFile);
    Shader(const char *vertexFile, const char *fragmentFile, const char *geometryFile);

    const GLuint getId() const;

    // Activates the Shader Program
    void activateShader();
    // Check for Shader Compile errors and print them
    void printCompileErrors(unsigned int shader, const char *type);
    // Deletes the Shader Program
    void deleteShader();

    void sendUniform(const char *uniform, GLuint value);
    void sendUniform(const char *uniform, bool value);
    void sendUniform(const char *uniform, GLfloat value);
    void sendUniform(const char *uniform, glm::vec3 vector);
    void sendUniform(const char *uniform, glm::vec4 vector);
    void sendUniform(const char *uniform, glm::mat4 matrix);

    virtual void drawModel(Model &model, Camera &camera) = 0;
};