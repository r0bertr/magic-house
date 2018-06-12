#include "lib/glad.h"
#include "Renderers/LandRenderer.hpp"

#include <cstdio>

LandRenderer::LandRenderer(Shader *shader, Texture **textures,
    Light *light) : Renderer(shader, textures, light) {
    initRenderData();
}

LandRenderer::~LandRenderer() {}

void LandRenderer::initRenderData() {
    GLfloat data[] = {
        -.5f,  .5f, 0.f, 0.f, 100.f, 0.f, 0.f, -1.f,
         .5f,  .5f, 0.f, 100.f, 100.f, 0.f, 0.f, -1.f,
         .5f, -.5f, 0.f, 100.f, 0.f, 0.f, 0.f, -1.f,

        -.5f,  .5f, 0.f, 0.f, 100.f, 0.f, 0.f, -1.f,
        -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f,
         .5f, -.5f, 0.f, 100.f, 0.f, 0.f, 0.f, -1.f,
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void *)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void *)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    numVertices = 6;
}