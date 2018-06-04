#include "lib/glad.h"
#include "Renderer.hpp"

#include <glm/gtx/transform.hpp>
#include <cstdio>

Renderer::Renderer(Shader *shader) {
    this->shader = shader;
    numVertices = 0;
    VBO = 0;
    VAO = 0;
    EBO = 0;
}

Renderer::~Renderer() {
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (EBO)
        glDeleteBuffers(1, &EBO);
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
}

void Renderer::draw(glm::mat4 projection, glm::mat4 view,
    glm::vec3 pos, glm::vec3 scale, glm::vec3 rotAxis, GLfloat rotate, glm::vec3 viewPos) {
    
    enable();
    glm::mat4 model(1.f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(rotate), rotAxis);

    shader->uniformM4("model", model);
    shader->uniformM4("view", view);
    shader->uniformM4("projection", projection);

    if (EBO)
        glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, NULL);
    else
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Renderer::enable() {
    shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(VAO);
}