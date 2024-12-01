#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

Texture::Texture(const char *texturePath, const char *texType, GLuint slot)
{
    type = texType;
    int widthImg, heightImg, numColCh;

    // Flip image
    stbi_set_flip_vertically_on_load(true);
    // Load image
    unsigned char *textureBytes = stbi_load(texturePath, &widthImg, &heightImg, &numColCh, 0);
    if (!textureBytes)
    {
        throw std::invalid_argument("Failed to load image: " + std::string(texturePath));
    }

    // Generate OpenGL texture object
    glGenTextures(1, &ID); // number of textures, pointer to texture objects
    // Assign the texture to a texture unit
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);

    // Configure the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Configure the way the texture repeats
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Assign the image to the OpenGL texture object
    if (numColCh == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBytes);
    }
    else if (numColCh == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, textureBytes);
    }
    else if (numColCh == 1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, textureBytes);
    }
    else
    {
        throw std::invalid_argument("Automatic Texture type recognition failed!");
    }

    // Generate Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // Delete the image data
    stbi_image_free(textureBytes);

    // Unbind the OpenGL texture object
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::TextureUnit(Shader &shader, const char *uniform, GLuint unit)
{
    // Gets the location of the uniform
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    // Shader needs to be activated before changing the value of a uniform
    shader.Activate();
    // Sets the value of the uniform
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + unit);
    // Bind the OpenGL texture object
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
    // Unbind the OpenGL texture object
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
    // Delete the object
    glDeleteTextures(1, &ID);
}