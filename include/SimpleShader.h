#pragma once

#include "Shader.h"

class SimpleShader : public Shader
{
public:
    // Contructor that build the Shaders from file
    SimpleShader();

    void drawModel(Model &model, Camera &camera) override;
};