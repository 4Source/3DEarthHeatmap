#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject(GLuint *indices, GLsizeiptr size)
{
    // Generate the Element Buffer Object with only 1 object
    glGenBuffers(1, &ID);

    // Bind the EBO specifying it is a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    // Add the indices to the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void ElementBufferObject::Bind()
{
    // Bind the EBO specifying it is a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ElementBufferObject::Unbind()
{
    // Bind to 0 so that you can't accidentally modify it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Important to unbind after VAO
}

void ElementBufferObject::Delete()
{
    // Delete the object
    glDeleteBuffers(1, &ID);
}
