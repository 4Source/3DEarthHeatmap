#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
    // Generate the Vertex Array Object with only 1 object
    glGenVertexArrays(1, &ID);
}

void VertexArrayObject::LinkVertexBufferObject(VertexBufferObject vbo, GLuint layout)
{
    vbo.Bind();

    // Configure the Vertex Attribute that OpenGL knows how to read the VBO
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
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
