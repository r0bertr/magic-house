#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

enum SpriteType {
    SPRITE_CUBE
};

class SpriteRenderer {
public:
    SpriteRenderer(Shader *shader);
    ~SpriteRenderer();

    void drawSprite(glm::mat4 projection, glm::mat4 view,
        glm::vec3 pos = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f), GLfloat rotate = 0.f,
        glm::vec3 color = glm::vec3(1.f), Texture *texture = NULL);

    void bind();

protected:
    Shader *shader;
    GLint count;
    GLuint VBO, VAO;

    virtual void initRenderData() = 0;
};