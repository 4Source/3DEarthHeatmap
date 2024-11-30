#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class VertexBufferObject
{
public:
    GLuint ID;
    // Constructor that generates the Vertex Buffer Object
    VertexBufferObject(GLfloat *vertices, GLsizeiptr size_t);

    // Binds the Vertex Buffer Object
    void Bind();
    // Unbinds the Vertex Buffer Object
    void Unbind();
    // Deletes the Vertex Buffer Object
    void Delete();
};
