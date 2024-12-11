#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(std::vector<Vertex> &vertices)
{
    // Generate the Vertex Buffer Object with only 1 object
    glGenBuffers(1, &mId);
    // Bind the VBO specifying it is a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, mId);
    // Add the vertices to the VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::bindBuffer()
{
    // Bind the VBO specifying it is a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void VertexBufferObject::unbindBuffer()
{
    // Bind to 0 so that you can't accidentally modify it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::deleteBuffer()
{
    // Delete the object
    glDeleteBuffers(1, &mId);
}
