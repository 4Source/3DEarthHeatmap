#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

Texture::Texture(const char *texturePath, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
    type = texType;
    int widthImg, heightImg, numColCh;

    // Flip image
    stbi_set_flip_vertically_on_load(true);
    // Load image
    unsigned char *textureBytes = stbi_load(texturePath, &widthImg, &heightImg, &numColCh, 0);
    if (!textureBytes)
    {
        std::cout << "\033[" << 91 << "m" << "Failed to load image: " << texturePath << "\033[0m" << "\n";
    }

    // Generate OpenGL texture object
    glGenTextures(1, &ID); // number of textures, pointer to texture objects
    // Assign the texture to a texture unit
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(type, ID);

    // Configure the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Configure the way the texture repeats
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Assign the image to the OpenGL texture object
    glTexImage2D(type, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, textureBytes);
    // Generate Mipmap
    glGenerateMipmap(type);

    // Delete the image data
    stbi_image_free(textureBytes);

    // Unbind the OpenGL texture object
    glBindTexture(texType, 0);
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
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    // Unbind the OpenGL texture object
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    // Delete the object
    glDeleteTextures(1, &ID);
}