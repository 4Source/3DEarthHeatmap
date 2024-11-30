#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(GLfloat *vertices, GLsizeiptr size)
{
    // Generate the Vertex Buffer Object with only 1 object
    glGenBuffers(1, &ID);
    // Bind the VBO specifying it is a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    // Add the vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexBufferObject::Bind()
{
    // Bind the VBO specifying it is a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBufferObject::Unbind()
{
    // Bind to 0 so that you can't accidentally modify it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::Delete()
{
    // Delete the object
    glDeleteBuffers(1, &ID);
}
