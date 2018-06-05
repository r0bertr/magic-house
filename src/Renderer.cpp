#include "lib/glad.h"
#include "Renderer.hpp"

#include <glm/gtx/transform.hpp>
#include <cstdio>
#include <string>

Renderer::Renderer(Shader *shader, Texture **textures, Light *light) {
    this->shader = shader;
    if (textures) {
        for (GLuint i = 0; i < 16; i++) {
            if (textures[i]) {
                this->textures[i] = textures[i];
            } else {
                this->textures[i] = NULL;
            }
        }
    }
    this->light = light;
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

void Renderer::draw(
        glm::mat4 projection,
        glm::mat4 view,
        glm::vec3 viewPos,
        glm::vec3 pos,
        glm::vec3 scale,
        glm::vec3 rotAxis,
        GLfloat rotate
    ) {

    enable();
    
    glm::mat4 model(1.f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(rotate), rotAxis);

    shader->uniformM4("model", model);
    shader->uniformM4("view", view);
    shader->uniformM4("projection", projection);
    shader->uniform3("viewPos", viewPos);

    for (GLuint i = 0; i < 16; i++) {
        if (textures[i]) {
            textures[i]->bind(i);
            std::string texName("texture");
            texName += std::to_string(i);
            shader->uniform1(texName.c_str(), i);
        }
    }

    if (light) {
        glm::vec3 lightColor = light->getColor();
        shader->uniform3("light.position", light->getPosition());
        shader->uniform3("light.direction", light->getDirection());
        shader->uniform3("light.ambient", light->getAmbient() * lightColor);
        shader->uniform3("light.diffuse", light->getDiffuse() * lightColor);
        shader->uniform3("light.specular", light->getSpecular() * lightColor);
    }

    if (EBO)
        glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, NULL);
    else
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Renderer::enable() {
    shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (EBO)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(VAO);
}