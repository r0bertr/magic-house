#pragma once
#include <vector>
#include <string>
#include <iostream>  
#include <fstream>
#include "Renderer.hpp"

using namespace std;

class SkyboxRenderer : public Renderer {
public:
    SkyboxRenderer(Shader *shader, Texture *texture);
    ~SkyboxRenderer();

    void draw(glm::mat4 projection, glm::mat4 view,
        glm::vec3 pos = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f), GLfloat rotate = 0.f, glm::vec3 viewPos = glm::vec3(0.f));

private:
    Texture *texture;
    void initRenderData();
};