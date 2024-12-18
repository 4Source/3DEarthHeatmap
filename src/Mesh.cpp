#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures)
{
    Mesh::mVertices = vertices;
    Mesh::mIndices = indices;
    Mesh::mTextures = textures;

    // Bind the Vertex array object
    mVAO.bindArray();

    // Generate the Vertex Buffer Object
    VertexBufferObject vbo(Mesh::mVertices);
    // Generate the Element Buffer Object
    ElementBufferObject ebo(Mesh::mIndices);

    // Link the attributes to the VAO
    mVAO.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);                   // Vertex position
    mVAO.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float))); // Normals
    mVAO.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float))); // Vertex color
    mVAO.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float))); // Vertex Texture coordinate
    // Unbind all to prevent modifying
    mVAO.unbindArray();
    vbo.unbindBuffer();
    ebo.unbindBuffer();
}

unsigned int Mesh::sizeOfTextures()
{
    return mTextures.size();
}

Texture &Mesh::getTexture(unsigned int index)
{
    return mTextures.at(index);
}

unsigned int Mesh::sizeOfVertices()
{
    return mVertices.size();
}

Vertex &Mesh::getVertex(unsigned int index)
{
    return mVertices.at(index);
}

void Mesh::bindMesh()
{
    // Bind Vertex array object
    mVAO.bindArray();
}

void Mesh::drawMesh(GLenum mode)
{
    // Draw the Triangles
    glDrawElements(mode, mIndices.size(), GL_UNSIGNED_INT, 0);
}