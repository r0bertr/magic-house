#pragma once
// #define STB_IMAGE_IMPLEMENTATION
#include <vector>
#include <string>
#include <iostream>  
#include <fstream>
#include "Renderer.hpp"
// #include "lib/stb_image.h"

using namespace std;

class SkyboxRenderer : public Renderer {
public:
    SkyboxRenderer(Shader *shader);
    ~SkyboxRenderer();

    void draw(glm::mat4 projection, glm::mat4 view,
    glm::vec3 pos, glm::vec3 scale, glm::vec3 rotAxis, GLfloat rotate);

private:
    unsigned int cubemapTexture;
    void initRenderData();
    unsigned int loadCubeMap(string *faces);
};