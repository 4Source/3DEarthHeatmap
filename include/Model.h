#pragma once

#include <single_include/nlohmann/json.hpp>
#include <vector>
#include <string>

#include "Shader.h"
#include "Camera.h"
#include "utils.h"
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
public:
    Model(const char *gltfFilePath);

    void drawModel(Shader &shader, Camera &camera);
    void drawNormals(Shader &shader);

protected:
    const char *pFile;
    std::vector<unsigned char> mData;
    json mJsonObj;

    std::vector<Mesh> mMeshes;
    std::vector<glm::vec3> mTranslationsMeshes;
    std::vector<glm::quat> mRotationsMeshes;
    std::vector<glm::vec3> mScalesMeshes;
    std::vector<glm::mat4> mMatricesMeshes;

    std::vector<std::string> mLoadedTexName;
    std::vector<Texture> mLoadedTex;

    void loadMesh(unsigned int indMesh);

    void traversNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture> getTextures();

    std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUV);

    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
