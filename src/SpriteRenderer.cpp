#include "lib/glad.h"
#include "SpriteRenderer.hpp"

#include <glm/gtx/transform.hpp>
#include <cstdio>

SpriteRenderer::SpriteRenderer(Shader *shader) {
    this->shader = shader;
    count = 0;
    VBO = 0;
    VAO = 0;
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void SpriteRenderer::drawSprite(glm::mat4 projection, glm::mat4 view,
    glm::vec3 pos, glm::vec3 scale, glm::vec3 rotAxis, GLfloat rotate,
    glm::vec3 color, Texture *texture) {

    glm::mat4 model(1.f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    // model = glm::rotate(model, glm::radians(rotate), rotAxis);

    shader->use();
    shader->uniformM4("model", model);
    shader->uniformM4("view", view);
    shader->uniformM4("projection", projection);
    shader->uniform3("spriteColor", color);
    if (texture) {
        texture->bind();
        shader->uniform1("image", texture->getID());
    } else {
        shader->uniform1("image", 0);
    }
    
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void SpriteRenderer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
}