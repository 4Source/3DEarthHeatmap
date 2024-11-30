#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class ElementBufferObject
{
public:
    GLuint ID;
    // Constructor that generates the Element Buffer Object
    ElementBufferObject(GLuint *indices, GLsizeiptr size);

    // Binds the Element Buffer Object
    void Bind();
    // Unbinds the Element Buffer Object
    void Unbind();
    // Deletes the Element Buffer Object
    void Delete();
};
