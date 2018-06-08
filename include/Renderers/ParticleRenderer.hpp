#pragma once

#include "Renderer.hpp"
#include <vector>

enum ParticleEmitterType {
    PARTICLE_POINT = 0,
    PARTICLE_BOX,
};

enum ParticleShape {
    PARTICLE_SHAPE_QUAD = 0,
    PARTICLE_SHAPE_CUBE,
};

class Particle {
public:
    Particle(ParticleShape shape,
        glm::vec3 position, glm::vec3 scale, glm::vec3 direction,
        glm::vec3 color, GLfloat rotate,
        GLfloat speed, GLfloat opacity, GLuint life);
    ~Particle();

    void bind();
    void move();
    bool dead();
    glm::mat4 getModel() const;
    glm::vec3 getColor() const;
    GLfloat getOpacity() const;

private:
    ParticleShape shape;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 direction;
    glm::vec3 color;
    GLfloat rotate;
    GLfloat speed;
    GLfloat opacity;
    GLuint life;
    GLuint numVertices;

    GLuint VBO, VAO;
};

class ParticleRenderer : public Renderer {
public:
    ParticleRenderer(Shader *shader, const GLchar *config, 
    Texture **textures = NULL, Light *light = NULL);
    ~ParticleRenderer();

    void draw(
        glm::mat4 projection,
        glm::mat4 view,
        glm::vec3 viewPos,
        glm::vec3 pos = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f),
        GLfloat rotate = 0.f,
        glm::vec4 color = glm::vec4(1.f)
    );
    
    void enable();

private:
    std::vector<Particle *> *particles;
    ParticleEmitterType type;
    ParticleShape shape;
    GLuint life;
    GLuint count;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 direction;
    GLfloat rotate;
    GLfloat speed;
    GLfloat opacity;
    glm::vec3 color;
    glm::vec3 randPos;
    glm::vec3 randScale;
    glm::vec3 randDir;
    GLfloat randRotate;
    GLfloat randSpeed;
    GLfloat randOpacity;
    GLuint randLife;

    void initRenderData();
    Particle *generate();
};