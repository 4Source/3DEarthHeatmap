#pragma once

#define GLEW_STATIC
#include <gl/glew.h>
#include <string>
#include <stdexcept>

class Texture
{
protected:
    GLuint mId;
    GLuint mUnit;
    const char *pType;
    GLenum target;

public:
    // Constructor that loads and generates the texture
    Texture() {};
    Texture(const char *texturePath, const char *texType, GLuint slot);
    Texture(GLubyte colors[], size_t colorsSize, const char *texType, GLuint slot);

    const char *getType() const;

    // Binds the Texture
    void bindTexture();
    // Unbinds the Texture
    void unbindTexture();
    // Deletes the Texture
    void deleteTexture();
};
