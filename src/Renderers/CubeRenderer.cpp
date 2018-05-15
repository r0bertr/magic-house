#include "lib/glad.h"
#include "Renderers/CubeRenderer.hpp"

#include <cstdio>

CubeRenderer::CubeRenderer(Shader *shader,
    Texture *texture) : Renderer(shader) {
    this->texture = texture;
    initRenderData();
}

void CubeRenderer::draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos,
    glm::vec3 scale, glm::vec3 rotAxis, GLfloat rotate, glm::vec3 color) {

    enable();
    shader->uniform3("color", color);
    Renderer::draw(projection, view, pos, scale, rotAxis, rotate);
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