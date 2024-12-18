#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

Texture::Texture(const char *texturePath, const char *texType, GLuint slot)
{
    pType = texType;
    int widthImg, heightImg, numColCh;
    target = GL_TEXTURE_2D;

    // Flip image
    stbi_set_flip_vertically_on_load(true);
    // Load image
    unsigned char *textureBytes = stbi_load(texturePath, &widthImg, &heightImg, &numColCh, 0);
    if (!textureBytes)
    {
        throw std::invalid_argument("Failed to load image: " + std::string(texturePath));
    }

    // Generate OpenGL texture object
    glGenTextures(1, &mId); // number of textures, pointer to texture objects
    // Assign the texture to a texture unit
    glActiveTexture(GL_TEXTURE0 + slot);
    mUnit = slot;
    glBindTexture(target, mId);

    // Configure the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Configure the way the texture repeats
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Assign the image to the OpenGL texture object
    if (numColCh == 4)
    {
        glTexImage2D(target, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBytes);
    }
    else if (numColCh == 3)
    {
        glTexImage2D(target, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, textureBytes);
    }
    else if (numColCh == 2)
    {
        glTexImage2D(target, 0, GL_RGBA, widthImg, heightImg, 0, GL_RG, GL_UNSIGNED_BYTE, textureBytes);
    }
    else if (numColCh == 1)
    {
        glTexImage2D(target, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, textureBytes);
    }
    else
    {
        throw std::invalid_argument("Automatic Texture type recognition failed!");
    }

    // Generate Mipmap
    glGenerateMipmap(target);

    // Delete the image data
    stbi_image_free(textureBytes);

    // Unbind the OpenGL texture object
    glBindTexture(target, 0);
}

Texture::Texture(GLubyte colors[], size_t colorsSize, const char *texType, GLuint slot)
{
    pType = texType;
    int numColCh = 4, widthImg = colorsSize / numColCh;
    target = GL_TEXTURE_1D;

    // Generate OpenGL texture object
    glGenTextures(1, &mId); // number of textures, pointer to texture objects
    // Assign the texture to a texture unit
    glActiveTexture(GL_TEXTURE0 + slot);
    mUnit = slot;
    glBindTexture(target, mId);

    // Upload the colorscale data
    glTexImage1D(target, 0, GL_RGBA, widthImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors);

    // Set texture parameters (important for sampling behavior)
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Clamp to edges
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // Linear filtering
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    // Linear filtering

    // Generate Mipmap
    glGenerateMipmap(target);

    // Unbind the OpenGL texture object
    glBindTexture(target, 0);
}

const char *Texture::getType() const
{
    return pType;
}

void Texture::bindTexture()
{
    glActiveTexture(GL_TEXTURE0 + mUnit);
    // Bind the OpenGL texture object
    glBindTexture(target, mId);
}

void Texture::unbindTexture()
{
    // Unbind the OpenGL texture object
    glBindTexture(target, 0);
}

void Texture::deleteTexture()
{
    // Delete the object
    glDeleteTextures(1, &mId);
}
