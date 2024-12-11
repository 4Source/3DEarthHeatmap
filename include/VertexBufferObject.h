#pragma once

#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

class VertexBufferObject
{
protected:
    GLuint ID;

public:
    // Constructor that generates the Vertex Buffer Object
    VertexBufferObject(std::vector<Vertex> &vertices);

    // Binds the Vertex Buffer Object
    void Bind();
    // Unbinds the Vertex Buffer Object
    void Unbind();
    // Deletes the Vertex Buffer Object
    void Delete();
};
