#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#include "VertexBufferObject.h"

class VertexArrayObject
{
protected:
    GLuint mId;

public:
    // Constructor that generates the Vertex Array Object
    VertexArrayObject();

    // Links the Vertex Buffer Object to the Vertex Array Object
    void linkAttrib(VertexBufferObject &vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset);
    // Binds the Vertex Array Object
    void bindArray();
    // Unbinds the Vertex Array Object
    void unbindArray();
    // Deletes the Vertex Array Object
    void deleteArray();
};
