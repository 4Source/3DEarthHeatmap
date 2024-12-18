#include "Model.h"

Model::Model(const char *gltfFilePath)
{
    std::cout << "Loading Model from: " << gltfFilePath << std::endl;

    // Read the content of the file
    std::string text = getFileContents(gltfFilePath);

    // Parse the text to a json object
    Model::mJsonObj = json::parse(text);

    // Store the file path for later usage
    Model::pFile = gltfFilePath;

    // Load the binary data from the .bin
    Model::mData = getData();
    if (mData.size() <= 0)
    {
        throw std::invalid_argument("Model contains no data!");
    }

    // Go throw all the nodes to collect the transformations for each level
    traversNode(0);
}

unsigned int Model::sizeOfMeshes()
{
    return mMeshes.size();
}

Mesh &Model::getMesh(unsigned int index)
{
    return mMeshes.at(index);
}

glm::mat4 Model::getMeshTranslation(unsigned int index)
{
    return glm::translate(glm::mat4(1.0f), mTranslationsMeshes.at(index));
}

glm::mat4 Model::getMeshRotation(unsigned int index)
{
    return glm::mat4_cast(mRotationsMeshes.at(index));
}

glm::mat4 Model::getMeshScale(unsigned int index)
{
    return glm::scale(glm::mat4(1.0f), mScalesMeshes.at(index));
}

glm::mat4 Model::getMeshMatrix(unsigned int index)
{
    return mMatricesMeshes.at(index);
}

void Model::loadMesh(unsigned int indMesh)
{
    // Get all accessor indices
    unsigned int posAccIndices = mJsonObj["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccIndices = mJsonObj["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccIndices = mJsonObj["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indicesAccIndices = mJsonObj["meshes"][indMesh]["primitives"][0]["indices"];

    // Use the accessor indices to get the components of the vertecies
    std::vector<glm::vec3> positions = groupFloatsVec3(getFloats(mJsonObj["accessors"][posAccIndices]));
    std::vector<glm::vec3> normals = groupFloatsVec3(getFloats(mJsonObj["accessors"][normalAccIndices]));
    std::vector<glm::vec2> texUVs = groupFloatsVec2(getFloats(mJsonObj["accessors"][texAccIndices]));

    // Combine all the vertex components
    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    // Get the indecies by the accassor indice
    std::vector<GLuint> indices = getIndices(mJsonObj["accessors"][indicesAccIndices]);
    // Get the textures
    std::vector<Texture> textures = getTextures();

    // Combine the vertecies, indices and textures into a mesh
    mMeshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traversNode(unsigned int nextNode, glm::mat4 matrix)
{
    // Current node
    json node = mJsonObj["nodes"][nextNode];

    // Get the translation if it exists
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    if (node.find("translation") != node.end())
    {
        float transValues[3];
        for (unsigned int i = 0; i < node["translation"].size(); i++)
        {
            transValues[i] = (node["translation"][i]);
        }
        translation = glm::make_vec3(transValues);
    }
    // Get the rotation if it exists
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end())
    {
        // Convert the order
        float rotValues[4] = {
            node["rotation"][3],
            node["rotation"][0],
            node["rotation"][1],
            node["rotation"][2],
        };

        rotation = glm::make_quat(rotValues);
    }
    // Get the scale if it exists
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    if (node.find("scale") != node.end())
    {
        float scaleValues[3];
        for (unsigned int i = 0; i < node["scale"].size(); i++)
        {
            scaleValues[i] = (node["scale"][i]);
        }
        scale = glm::make_vec3(scaleValues);
    }
    // Get matrix if it exists
    glm::mat4 matrixNode = glm::mat4(1.0f);
    if (node.find("matrix") != node.end())
    {
        float matrixValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++)
        {
            matrixValues[i] = (node["matrix"][i]);
        }
        matrixNode = glm::make_mat4(matrixValues);
    }

    // Create matrices from translation, rotation and scale
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 rot = glm::mat4_cast(rotation);
    glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

    // Combine all metrecies to one
    glm::mat4 matNextNode = matrix * matrixNode * trans * rot * sca;

    // Check if the node contains a mesh
    if (node.find("mesh") != node.end())
    {
        // Store the transformations
        mTranslationsMeshes.push_back(translation);
        mRotationsMeshes.push_back(rotation);
        mScalesMeshes.push_back(scale);
        mMatricesMeshes.push_back(matNextNode);

        // Load the mesh
        loadMesh(node["mesh"]);
    }

    // Check if the node contains children
    if (node.find("children") != node.end())
    {
        // For each children rekursive travers the nodes
        for (unsigned int i = 0; i < node["children"].size(); i++)
        {
            traversNode(node["children"][i], matNextNode);
        }
    }
}

std::vector<unsigned char> Model::getData()
{
    // Get the uri (filename) of the .bin file
    std::string uri = mJsonObj["buffers"][0]["uri"];

    // Make a copy of the file string
    std::string fileStr = std::string(pFile);
    // Cut of the filename and extension
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
    // Read the raw content of the .bin file
    std::string bytesText = getFileContents((fileDirectory + uri).c_str());

    // Transform the raw text data into bytes and put them in a vector
    return std::vector<unsigned char>(bytesText.begin(), bytesText.end());
}

std::vector<float> Model::getFloats(json accessor)
{
    std::vector<float> floatVec;

    // Get the properties of the accessor
    unsigned int buffViewIndices = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    // Get the properties of the bufferView
    json bufferView = mJsonObj["bufferViews"][buffViewIndices];
    unsigned int byteOffset = bufferView["byteOffset"];

    // Interpret the type
    unsigned int numPerVert;
    if (type == "SCALAR")
        numPerVert = 1;
    else if (type == "VEC2")
        numPerVert = 2;
    else if (type == "VEC3")
        numPerVert = 3;
    else if (type == "VEC4")
        numPerVert = 4;
    else
        throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3 or VEC4)");

    // Go over all the bytes in the data at the correct place using the properies
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;
    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
    {
        unsigned char bytes[] = {mData[i++], mData[i++], mData[i++], mData[i++]};
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
    std::vector<GLuint> indices;

    // Get the properties of the accessor
    unsigned int buffViewIndices = accessor.value("bufferView", 0);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];

    // Get the properties of the bufferView
    json bufferView = mJsonObj["bufferViews"][buffViewIndices];
    unsigned int byteOffset = bufferView.value("byteOffset", 0);

    // Get indices regards to their type:
    // unsigned int
    unsigned int beginningOfData = byteOffset + accByteOffset;
    if (componentType == 5125)
    {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
        {
            unsigned char bytes[] = {mData[i++], mData[i++], mData[i++], mData[i++]};
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(unsigned int));
            indices.push_back((GLuint)value);
        }
    }
    // unsigned short
    else if (componentType == 5123)
    {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
        {
            unsigned char bytes[] = {mData[i++], mData[i++]};
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    }
    // short
    else if (componentType == 5122)
    {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
        {
            unsigned char bytes[] = {mData[i++], mData[i++]};
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
        }
    }

    return indices;
}

std::vector<Texture> Model::getTextures()
{
    std::vector<Texture> textures;

    std::string fileStr = std::string(pFile);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    // Go over all images
    for (unsigned int i = 0; i < mJsonObj["images"].size(); i++)
    {
        // uri of current texture
        std::string texPath = mJsonObj["images"][i]["uri"];

        // Check if the texture has already been loaded
        bool skip = false;
        for (unsigned int j = 0; j < mLoadedTexName.size(); j++)
        {
            if (mLoadedTexName[j] == texPath)
            {
                textures.push_back(mLoadedTex[j]);
                skip = true;
                break;
            }
        }

        // If the texture has not been loaded
        if (!skip)
        {
            // Load diffuse texture
            if (texPath.find("baseColor") != std::string::npos)
            {
                Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", mLoadedTex.size());
                textures.push_back(diffuse);
                mLoadedTex.push_back(diffuse);
                mLoadedTexName.push_back(texPath);
            }
            // Load specular texture
            else if (texPath.find("metallicRoughness") != std::string::npos)
            {
                Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", mLoadedTex.size());
                textures.push_back(specular);
                mLoadedTex.push_back(specular);
                mLoadedTexName.push_back(texPath);
            }
        }
    }

    return textures;
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUV)
{
    std::vector<Vertex> vertices;
    for (int i = 0; i < positions.size(); i++)
    {
        vertices.push_back(Vertex{positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), texUV[i]});
    }
    return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
    std::vector<glm::vec2> vectors;
    for (int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
    std::vector<glm::vec3> vectors;
    for (int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
    std::vector<glm::vec4> vectors;
    for (int i = 0; i < floatVec.size(); i)
    {
        vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}
