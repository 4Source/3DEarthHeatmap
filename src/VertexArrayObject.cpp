#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
    // Generate the Vertex Array Object with only 1 object
    glGenVertexArrays(1, &ID);
}

void VertexArrayObject::LinkAttrib(VertexBufferObject &vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset)
{
    vbo.Bind();

    // Configure the Vertex Attribute that OpenGL knows how to read the VBO
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    // Enable the Vertex Attribute
    glEnableVertexAttribArray(layout);

    vbo.Unbind();
}

void VertexArrayObject::Bind()
{
    // Make it to the current Vertex Array Object
    glBindVertexArray(ID);
}

void VertexArrayObject::Unbind()
{
    // Bind to 0 so that you can't accidentally modify it
    glBindVertexArray(0);
}

void VertexArrayObject::Delete()
{
    // Delete the object
    glDeleteVertexArrays(1, &ID);
}
