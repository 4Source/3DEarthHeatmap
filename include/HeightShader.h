#pragma once

#include <limits>
#include <vector>

#include "Shader.h"
#include "Texture.h"

class HeightShader : public Shader
{
private:
    float mMinDistance, mMaxDistance;
    Texture mColorGradient;

public:
    // Contructor that build the Shaders from file
    HeightShader();
    HeightShader(std::vector<GLubyte> colors);

    void drawModel(Model &model, Camera &camera) override;
};