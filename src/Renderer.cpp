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
        GLfloat rotate,
        glm::vec4 color,
        glm::vec3 collisionSize
    ) {
	enable();
    
	glm::mat4 model(1.f);
	glm::mat4 lightSpaceMatrix(1.f), lightView(1.f);
    glm::vec4 anchor(0.f, 0.f, 0.f, 1.f);

    model = glm::translate(model, pos);
    anchor = model * anchor;
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(rotate), rotAxis);

    if (collisionSize != glm::vec3(0.f))
        CollisionDetector::setValue(
            std::to_string(anchor.x + anchor.y + anchor.z).c_str(),
            anchor, collisionSize
        );

    shader->uniformM4("model", model);
    shader->uniformM4("view", view);
    shader->uniformM4("projection", projection);
    shader->uniform3("viewPos", viewPos);
    shader->uniform4("color", color);

    for (GLuint i = 0; i < 16; i++) {
        if (textures[i]) {
            textures[i]->bind(i);
            std::string texName("texture");
            texName += std::to_string(i);
            shader->uniform1(texName.c_str(), i);
        }
    }
    shader->uniform1("hasTexture", textures[0] != NULL);

    if (light) {
        glm::vec3 lightColor = light->getColor();
        shader->uniform3("light.position", light->getPosition());
        shader->uniform3("light.direction", light->getDirection());
        shader->uniform3("light.ambient", light->getAmbient() * lightColor);
        shader->uniform3("light.diffuse", light->getDiffuse() * lightColor);
        shader->uniform3("light.specular", light->getSpecular() * lightColor);
		lightView = glm::lookAt(light->getPosition(), glm::vec3(0.f),
            glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = light->getProjection() * lightView;
		shader->uniformM4("lightSpaceMatrix", lightSpaceMatrix);
    }
    if (EBO)
        glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, NULL);
    else
        glDrawArrays(GL_TRIANGLES, 0, numVertices);

}

void Renderer::setShader(Shader *shader) {
    
    this->shader = shader;

}

void Renderer::setTexture(GLuint index, Texture *texture) {

    this->textures[index] = texture;

}

void Renderer::enable() {
    shader->use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
}