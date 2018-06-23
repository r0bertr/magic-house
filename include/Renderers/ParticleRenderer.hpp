#pragma once

#include "lib/cJSON.h"
#include "lib/glad.h"
#include "Renderer.hpp"
#include "Model.hpp"

#include <vector>
#include <glm/glm.hpp>

enum ParticleShape {
    PARTICLE_SHAPE_SPHERE = 0,
    PARTICLE_SHAPE_RECT,
};

template<class T>
struct RandValue {
    T mean;
    T variance;

    RandValue();
    RandValue(T m, T v);
    T eval() const;
};

struct ParticleAttributes {

    RandValue<GLint> numParticles;
    RandValue<glm::vec3> initPosition;
    RandValue<glm::vec3> initDirection;
    RandValue<GLfloat> initSpeed;
    RandValue<glm::vec3> initSize;
    RandValue<glm::vec4> initColor;
    ParticleShape initShape;
    GLuint initLife;
    bool gravity;
    glm::vec3 gravityDir;
    GLfloat gravityMag;

    ParticleAttributes();
    ParticleAttributes(const GLchar *path);
    void setAttributes(cJSON *parameters);
    void dump(const GLchar *path);

};

class ParticleRenderer : public Renderer {
public:
    ParticleRenderer(Shader *shader, const GLchar *config, Light *light = NULL);
    ParticleRenderer(const ParticleRenderer &r);
    ~ParticleRenderer();

    virtual void draw(
        glm::mat4 projection,
        glm::mat4 view,
        glm::vec3 viewPos,
        glm::vec3 pos = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f),
        GLfloat rotate = 0.f,
        glm::vec4 color = glm::vec4(1.f),
        glm::vec3 collisionSize = glm::vec3(0.f)
    );
    
    virtual void enable();
    virtual void setShader(Shader *shader);
    virtual bool dead();

    ParticleAttributes *getAttributes();

protected:
    ParticleAttributes *attributes;

private:
    std::vector<ParticleRenderer *> children;

    virtual void initRenderData();
    void spawn();
};

class Particle : public ParticleRenderer {
public:
    Particle(const ParticleRenderer &r);
    ~Particle();

    void draw(
        glm::mat4 projection,
        glm::mat4 view,
        glm::vec3 viewPos,
        glm::vec3 pos = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f),
        GLfloat rotate = 0.f,
        glm::vec4 color = glm::vec4(1.f),
        glm::vec3 collisionSize = glm::vec3(0.f)
    );
    void animate();
    void enable();
    bool dead();

private:
    Model *model;
    glm::vec3 position;
    glm::vec3 direction;
    GLfloat speed;
    glm::vec4 color;
    glm::vec3 size;
    GLuint life;
    void initRenderData();
};
