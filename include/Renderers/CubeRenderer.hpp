#pragma once

#include "Renderer.hpp"

class CubeRenderer : public Renderer {
public:
    CubeRenderer(Shader *shader, Texture *texture = NULL);
    ~CubeRenderer();

    void draw(glm::mat4 projection, glm::mat4 view,
        glm::vec3 pos = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f), GLfloat rotate = 0.f,
        glm::vec3 color = glm::vec3(1.f));

private:
    Texture *texture;

    void initRenderData();
};
