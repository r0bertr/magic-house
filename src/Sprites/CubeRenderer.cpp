#include "lib/glad.h"
#include "Sprites/CubeRenderer.hpp"

#include <cstdio>

CubeRenderer::CubeRenderer(Shader *shader) : SpriteRenderer(shader) {
    initRenderData();
}

void CubeRenderer::initRenderData() {
    GLfloat data[] = {
        // back
        -.5f, -.5f, -.5f, 0.f, 0.f,
         .5f, -.5f, -.5f, 1.f, 0.f,
         .5f,  .5f, -.5f, 1.f, 1.f,
         .5f,  .5f, -.5f, 1.f, 1.f,
        -.5f,  .5f, -.5f, 0.f, 1.f,
        -.5f, -.5f, -.5f, 0.f, 0.f,

        // front
        -.5f, -.5f,  .5f, 0.f, 0.f,
         .5f, -.5f,  .5f, 1.f, 0.f,
         .5f,  .5f,  .5f, 1.f, 1.f,
         .5f,  .5f,  .5f, 1.f, 1.f,
        -.5f,  .5f,  .5f, 0.f, 1.f,
        -.5f, -.5f,  .5f, 0.f, 0.f,

        // left
        -.5f,  .5f,  .5f, 1.f, 0.f,
        -.5f,  .5f, -.5f, 1.f, 1.f,
        -.5f, -.5f, -.5f, 0.f, 1.f,
        -.5f, -.5f, -.5f, 0.f, 1.f,
        -.5f, -.5f,  .5f, 0.f, 0.f,
        -.5f,  .5f,  .5f, 1.f, 0.f,

        // right
         .5f,  .5f,  .5f, 0.f, 1.f,
         .5f,  .5f, -.5f, 1.f, 1.f,
         .5f, -.5f, -.5f, 1.f, 0.f,
         .5f, -.5f, -.5f, 1.f, 0.f,
         .5f, -.5f,  .5f, 0.f, 0.f,
         .5f,  .5f,  .5f, 0.f, 1.f,

        // bottom
        -.5f, -.5f, -.5f, 0.f, 1.f,
         .5f, -.5f, -.5f, 1.f, 1.f,
         .5f, -.5f,  .5f, 1.f, 0.f,
         .5f, -.5f,  .5f, 1.f, 0.f,
        -.5f, -.5f,  .5f, 0.f, 0.f,
        -.5f, -.5f, -.5f, 0.f, 1.f,

        // top
        -.5f,  .5f, -.5f, 0.f, 1.f,
         .5f,  .5f, -.5f, 1.f, 1.f,
         .5f,  .5f,  .5f, 1.f, 0.f,
         .5f,  .5f,  .5f, 1.f, 0.f,
        -.5f,  .5f,  .5f, 0.f, 0.f,
        -.5f,  .5f, -.5f, 0.f, 1.f,
    };
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
        (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    count = 36;
}