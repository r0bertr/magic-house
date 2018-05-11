#pragma once

#include "SpriteRenderer.hpp"

class CubeRenderer : public SpriteRenderer {
public:
    CubeRenderer(Shader *shader);
    ~CubeRenderer();

private:
    void initRenderData();
};