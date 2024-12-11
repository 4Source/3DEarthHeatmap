#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#include "Shader.h"

class Texture
{
protected:
    GLuint mId;
    GLuint mUnit;
    const char *pType;

public:
    // Constructor that loads and generates the texture
    Texture(const char *texturePath, const char *texType, GLuint slot);

    const char *getType() const;

    void sendTexture(Shader &shader, const char *uniform, GLuint unit);
    // Binds the Texture
    void bindTexture();
    // Unbinds the Texture
    void unbindTexture();
    // Deletes the Texture
    void deleteTexture();
};
