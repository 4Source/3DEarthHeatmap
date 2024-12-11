#include "ElementBufferObject.h"

ElementBufferObject::ElementBufferObject(std::vector<GLuint> &indices)
{
    // Generate the Element Buffer Object with only 1 object
    glGenBuffers(1, &mId);

    // Bind the EBO specifying it is a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
    // Add the indices to the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void ElementBufferObject::bindBuffer()
{
    // Bind the EBO specifying it is a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
}

void ElementBufferObject::unbindBuffer()
{
    // Bind to 0 so that you can't accidentally modify it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Important to unbind after VAO
}

void ElementBufferObject::deleteBuffer()
{
    // Delete the object
    glDeleteBuffers(1, &mId);
}