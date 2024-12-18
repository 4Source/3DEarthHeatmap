#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <iostream>

#include "utils.h"

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
};