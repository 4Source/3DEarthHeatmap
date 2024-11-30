#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#include "Shader.h"

class Texture
{
public:
    GLuint ID;
    GLenum type;
    // Constructor that loads and generates the texture
    Texture(const char *texturePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    void TextureUnit(Shader &shader, const char *uniform, GLuint unit);
    // Binds the Texture
    void Bind();
    // Unbinds the Texture
    void Unbind();
    // Deletes the Texture
    void Delete();
};
