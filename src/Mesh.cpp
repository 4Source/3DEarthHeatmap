#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    // Bind the Vertex array object
    vao.Bind();

    // Generate the Vertex Buffer Object
    VertexBufferObject vbo(Mesh::vertices);
    // Generate the Element Buffer Object
    ElementBufferObject ebo(Mesh::indices);

    // Link the attributes to the VAO
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);                   // Vertex position
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float))); // Normals
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float))); // Vertex color
    vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float))); // Vertex Texture coordinate
    // Unbind all to prevent modifying
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Mesh::Draw(Shader &shader, Camera &camera)
{
    // Activete shader
    shader.Activate();
    // Bind Vertex array object
    vao.Bind();

    // Index of diffuse or Specular Textures
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    // Check the type of texture and
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;

        if (type == "diffuse")
        {
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular")
        {
            num = std::to_string(numSpecular++);
        }
        else
        {
            std::cout << "\033[" << 91 << "m" << "Unknown type of Texture: " << type << "\033[0m" << std::endl;
            continue;
        }
        textures[i].TextureUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }

    // Export the camPos to the Shader
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
    // Export the camMatrix to the Shader
    camera.Matrix(shader, "camMatrix");

    // Draw the Triangles
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
