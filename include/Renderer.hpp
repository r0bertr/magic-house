#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

enum RendererType {
    RENDERER_MESH,
    RENDERER_LAND,
    RENDERER_SKYBOX,
};

class Renderer {
public:
    Renderer(Shader *shader);
    ~Renderer();

    virtual void draw(glm::mat4 projection, glm::mat4 view,
        glm::vec3 pos = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f), GLfloat rotate = 0.f);

    virtual void enable();

protected:
    Shader *shader;
    GLuint numVertices;
    GLuint VBO, VAO, EBO;

    virtual void initRenderData() = 0;
};
