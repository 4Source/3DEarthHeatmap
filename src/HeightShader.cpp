#include "HeightShader.h"

HeightShader::HeightShader() : HeightShader({29, 72, 119, 255, 251, 176, 33, 255, 246, 136, 56, 255, 238, 62, 50, 255}) {}

HeightShader::HeightShader(std::vector<GLubyte> colors) : Shader("../shader/HeightShader.vs", "../shader/HeightShader.fs")
{
    if (colors.size() % 4 != 0)
    {
        throw std::logic_error("Colors for gradient aren't a multble of 4 (rgba)");
    }

    if (colors.size() < 8)
    {
        throw std::logic_error("Colors for gradient aren't enought values at least 8 values");
    }

    mMinDistance = std::numeric_limits<float>::max();
    mMaxDistance = std::numeric_limits<float>::min();

    mColorGradient = Texture(colors.data(), colors.size(), "gradiant", 0);
}

void HeightShader::drawModel(Model &model, Camera &camera)
{
    glm::vec3 center = glm::vec3(0.0f);

    if (mMaxDistance < mMinDistance)
    {
        for (unsigned int i = 0; i < model.sizeOfMeshes(); i++)
        {
            Mesh &mesh = model.getMesh(i);

            for (unsigned int j = 0; j < mesh.sizeOfVertices(); j++)
            {
                Vertex &vertex = mesh.getVertex(j);

                float length = glm::length(vertex.position - center);

                if (length > mMaxDistance)
                {
                    mMaxDistance = length;
                }
                if (length < mMinDistance)
                {
                    mMinDistance = length;
                }
            }
        }
    }
    // Activete shader
    activateShader();
    // Draw each mesh in the model with the transformations
    for (unsigned int i = 0; i < model.sizeOfMeshes(); i++)
    {
        Mesh &mesh = model.getMesh(i);
        mesh.bindMesh();

        std::string type = mColorGradient.getType();

        if (type == "gradiant")
        {
            sendUniform((type + "0").c_str(), (GLuint)0);
            mColorGradient.bindTexture();
        }
        else
        {
            throw std::invalid_argument("Unknown type of Texture: " + type);
        }

        sendUniform("center", center);
        sendUniform("minDistance", mMinDistance);
        sendUniform("maxDistance", mMaxDistance);

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