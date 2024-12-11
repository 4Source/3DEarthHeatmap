#pragma once

#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>

class ElementBufferObject
{
protected:
    GLuint mId;

public:
    // Constructor that generates the Element Buffer Object
    ElementBufferObject(std::vector<GLuint> &indices);

    // Binds the Element Buffer Object
    void bindBuffer();
    // Unbinds the Element Buffer Object
    void unbindBuffer();
    // Deletes the Element Buffer Object
    void deleteBuffer();
};
