#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string get_file_contents(const char *filename);

class Shader
{
public:
    GLuint ID;
    // Contructor that build the Shaders from file
    Shader(const char *vertexFile, const char *fragmentFile);

    // Activates the Shader Program
    void Activate();
    // Deletes the Shader Program
    void Delete();
    // Check for Shader Compile errors and print them
    void CompileErrors(unsigned int shader, const char *type);
};