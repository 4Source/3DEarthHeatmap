#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
    // Generate the Vertex Array Object with only 1 object
    glGenVertexArrays(1, &mId);
}

void VertexArrayObject::linkAttrib(VertexBufferObject &vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset)
{
    vbo.bindBuffer();

    // Configure the Vertex Attribute that OpenGL knows how to read the VBO
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    // Enable the Vertex Attribute
    glEnableVertexAttribArray(layout);

    vbo.unbindBuffer();
}

void VertexArrayObject::bindArray()
{
    // Make it to the current Vertex Array Object
    glBindVertexArray(mId);
}

void VertexArrayObject::unbindArray()
{
    // Bind to 0 so that you can't accidentally modify it
    glBindVertexArray(0);
}

void VertexArrayObject::deleteArray()
{
    // Delete the object
    glDeleteVertexArrays(1, &mId);
}
