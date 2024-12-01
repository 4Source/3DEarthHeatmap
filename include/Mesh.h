#pragma once

#include <string>
#include <vector>

#include "VertexArrayObject.h"
#include "ElementBufferObject.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VertexArrayObject vao;


    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
    
    void Draw(Shader& shader, Camera& camera);
};
