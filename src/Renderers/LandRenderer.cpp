#include "lib/glad.h"
#include "Renderers/LandRenderer.hpp"

#include <cstdio>

LandRenderer::LandRenderer(Shader *shader,
    Texture *texture) : Renderer(shader) {
    this->texture = texture;
    initRenderData();
}

LandRenderer::~LandRenderer() {}

void LandRenderer::draw(glm::mat4 projection, glm::mat4 view,
    glm::vec3 pos, glm::vec3 scale, glm::vec3 rotAxis, GLfloat rotate, glm::vec3 viewPos) {
    
    enable();
    texture->bind();
    shader->uniform1("texture0", 0);
    // shader->uniform4("color", glm::vec4(1.f));
	shader->uniform3("viewPos", viewPos);

	// directional light
	shader->uniform3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader->uniform3("dirLight.ambient", 0.6f, 0.6f, 0.6f);
	shader->uniform3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
	shader->uniform3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	// material properties
	shader->uniform3("material.specular", 0.1f, 0.1f, 0.1f);
	shader->uniform1("material.shininess", 32.0f);

    Renderer::draw(projection, view, pos, scale, rotAxis, rotate);
}

void LandRenderer::initRenderData() {
    GLfloat data[] = {
        -.5f,  .5f, 0.f, 0.f, 100.f,
         .5f,  .5f, 0.f, 100.f, 100.f,
         .5f, -.5f, 0.f, 100.f, 0.f,

        -.5f,  .5f, 0.f, 0.f, 100.f,
        -.5f, -.5f, 0.f, 0.f, 0.f,
         .5f, -.5f, 0.f, 100.f, 0.f,
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
    numVertices = 6;
}