#include "SimpleShader.h"

SimpleShader::SimpleShader() : Shader("../shader/SimpleShader.vs", "../shader/SimpleShader.fs") {}

void SimpleShader::drawModel(Model &model, Camera &camera)
{
        // Activete shader
    activateShader();
    // Draw each mesh in the model with the transformations
    for (unsigned int i = 0; i < model.sizeOfMeshes(); i++)
    {
        Mesh &mesh = model.getMesh(i);
        mesh.bindMesh();

        // Index of diffuse or Specular Textures
        unsigned int numDiffuse = 0;
        unsigned int numSpecular = 0;

        // Check the type of texture
        for (unsigned int i = 0; i < mesh.sizeOfTextures(); i++)
        {
            std::string num;
            std::string type = mesh.getTexture(i).getType();

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
            sendUniform((type + num).c_str(), i);
            mesh.getTexture(i).bindTexture();
        }

        // Export the camPos to the Shader
        glUniform3f(glGetUniformLocation(getId(), "camPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
        // Export the camMatrix to the Shader
        sendUniform("camMatrix", camera.getCameraMatrix());

        // Export the mesh transformations to shader
        sendUniform("translation", model.getMeshTranslation(i));
        sendUniform("rotation", model.getMeshRotation(i));
        sendUniform("scale", model.getMeshScale(i));
        sendUniform("model", model.getMeshMatrix(i));

        // Draw the mesh using triangles
        mesh.drawMesh(GL_TRIANGLES);
    }
}
