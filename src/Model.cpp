#include "Model.h"

Model::Model(const char *gltfFilePath)
{
    std::cout << "Loading Model from: " << gltfFilePath << std::endl;

    // Read the content of the file
    std::string text = get_file_contents(gltfFilePath);

    // Parse the text to a json object
    Model::jsonObj = json::parse(text);

    // Store the file path for later usage
    Model::file = gltfFilePath;

    // Load the binary data from the .bin
    Model::data = getData();
    if (data.size() <= 0)
    {
        throw std::invalid_argument("Model contains no data!");
    }

    // Go throw all the nodes to collect the transformations for each level
    traversNode(0);
}

void Model::Draw(Shader &shader, Camera &camera)
{
    // Draw each mesh in the model with the transformations
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i], translationsMeshes[i], rotationsMeshes[i], scalesMeshes[i]);
    }
}

void Model::loadMesh(unsigned int indMesh)
{
    // Get all accessor indices
    unsigned int posAccIndices = jsonObj["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccIndices = jsonObj["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccIndices = jsonObj["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indicesAccIndices = jsonObj["meshes"][indMesh]["primitives"][0]["indices"];

    // Use the accessor indices to get the components of the vertecies
    std::vector<glm::vec3> positions = groupFloatsVec3(getFloats(jsonObj["accessors"][posAccIndices]));
    std::vector<glm::vec3> normals = groupFloatsVec3(getFloats(jsonObj["accessors"][normalAccIndices]));
    std::vector<glm::vec2> texUVs = groupFloatsVec2(getFloats(jsonObj["accessors"][texAccIndices]));

    // Combine all the vertex components
    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    // Get the indecies by the accassor indice
    std::vector<GLuint> indices = getIndices(jsonObj["accessors"][indicesAccIndices]);
    // Get the textures
    std::vector<Texture> textures = getTextures();

    // Combine the vertecies, indices and textures into a mesh
    meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traversNode(unsigned int nextNode, glm::mat4 matrix)
{
    // Current node
    json node = jsonObj["nodes"][nextNode];

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
        translationsMeshes.push_back(translation);
        rotationsMeshes.push_back(rotation);
        scalesMeshes.push_back(scale);
        matricesMeshes.push_back(matNextNode);

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
    std::string uri = jsonObj["buffers"][0]["uri"];

    // Make a copy of the file string
    std::string fileStr = std::string(file);
    // Cut of the filename and extension
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
    // Read the raw content of the .bin file
    std::string bytesText = get_file_contents((fileDirectory + uri).c_str());

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
    json bufferView = jsonObj["bufferViews"][buffViewIndices];
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
        unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
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
    json bufferView = jsonObj["bufferViews"][buffViewIndices];
    unsigned int byteOffset = bufferView["byteOffset"];

    // Get indices regards to their type:
    // unsigned int
    unsigned int beginningOfData = byteOffset + accByteOffset;
    if (componentType == 5125)
    {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
        {
            unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
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
            unsigned char bytes[] = {data[i++], data[i++]};
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
            unsigned char bytes[] = {data[i++], data[i++]};
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

    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    // Go over all images
    for (unsigned int i = 0; i < jsonObj["images"].size(); i++)
    {
        // uri of current texture
        std::string texPath = jsonObj["images"][i]["uri"];

        // Check if the texture has already been loaded
        bool skip = false;
        for (unsigned int j = 0; j < loadedTexName.size(); j++)
        {
            if (loadedTexName[j] == texPath)
            {
                textures.push_back(loadedTex[j]);
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
                Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
                textures.push_back(diffuse);
                loadedTex.push_back(diffuse);
                loadedTexName.push_back(texPath);
            }
            // Load specular texture
            else if (texPath.find("metallicRoughness") != std::string::npos)
            {
                Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
                textures.push_back(specular);
                loadedTex.push_back(specular);
                loadedTexName.push_back(texPath);
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
