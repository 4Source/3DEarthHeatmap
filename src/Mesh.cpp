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

void Mesh::drawMesh(Shader &shader, Camera &camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
    // Activete shader
    shader.activateShader();
    // Bind Vertex array object
    mVAO.bindArray();

    // Index of diffuse or Specular Textures
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    // Check the type of texture
    for (unsigned int i = 0; i < mTextures.size(); i++)
    {
        std::string num;
        std::string type = mTextures[i].getType();

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
            throw std::invalid_argument("Unknown type of Texture: " + type);
        }
        mTextures[i].sendTexture(shader, (type + num).c_str(), i);
        mTextures[i].bindTexture();
    }

    // Export the camPos to the Shader
    glUniform3f(glGetUniformLocation(shader.getId(), "camPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    // Export the camMatrix to the Shader
    camera.sendMatrix(shader, "camMatrix");

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca, scale);

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "scale"), 1, GL_FALSE, glm::value_ptr(sca));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(matrix));

    // Draw the Triangles
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::drawNormals(Shader &shader, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
    // Activete shader
    shader.activateShader();
    // Bind Vertex array object
    mVAO.bindArray();

    // Draw the Triangles
    glDrawElements(GL_POINTS, mIndices.size(), GL_UNSIGNED_INT, 0);
}
