#pragma once

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Texture.h"

class Mesh
{
protected:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    std::vector<Texture> mTextures;

    VertexArrayObject mVAO;

public:
    Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);

    unsigned int sizeOfTextures();
    Texture &getTexture(unsigned int index);
    unsigned int sizeOfVertices();
    Vertex& getVertex(unsigned int index);

    void bindMesh();
    void drawMesh(GLenum mode);
};
