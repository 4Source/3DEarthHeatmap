#pragma once

#include <string>
#include <vector>

#include "VertexArrayObject.h"
#include "ElementBufferObject.h"
#include "Camera.h"
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

    void drawMesh(Shader &shader, Camera &camera, glm::mat4 matrix = glm::mat4(1.0f), glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
    void drawNormals(Shader &shader, glm::mat4 matrix = glm::mat4(1.0f), glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f), glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};
