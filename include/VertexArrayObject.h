#pragma once

#define GLEW_STATIC
#include <gl/glew.h>
#include "VertexBufferObject.h"

class VertexArrayObject
{
public:
    GLuint ID;
    // Constructor that generates the Vertex Array Object
    VertexArrayObject();

    // Links the Vertex Buffer Object to the Vertex Array Object
    void LinkVertexBufferObject(VertexBufferObject vbo, GLuint layout);
    // Binds the Vertex Array Object
    void Bind();
    // Unbinds the Vertex Array Object
    void Unbind();
    // Deletes the Vertex Array Object
    void Delete();
};
