#pragma once

#include "Renderer.hpp"

class LandRenderer : public Renderer {
public:
    LandRenderer(Shader *shader, Texture **textures = NULL, Light *light = NULL);
    ~LandRenderer();

private:
    void initRenderData();
};
