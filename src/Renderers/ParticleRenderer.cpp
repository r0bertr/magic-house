#include "lib/glad.h"
#include "lib/cJSON.h"
#include "Renderers/ParticleRenderer.hpp"

#include <glm/gtx/transform.hpp>
#include <cstdlib>
#include <string>

glm::vec3 parseVec3(char *raw) {
    std::string str(raw);
    GLfloat x = (GLfloat)atof(str.substr(0, str.find(",")).c_str());
    str.erase(0, str.find(",") + 1);
    GLfloat y = (GLfloat)atof(str.substr(0, str.find(",")).c_str());
    str.erase(0, str.find(",") + 1);
    GLfloat z = (GLfloat)atof(str.c_str());
    return glm::vec3(x, y, z);
}

GLfloat randf(GLfloat a) {
    GLfloat randNum = (GLfloat)rand();
    return ((randNum / (GLfloat)RAND_MAX) * 2 - 1) * a;
}

Particle::Particle(ParticleShape shape,
    glm::vec3 position, glm::vec3 scale, glm::vec3 direction,
    glm::vec3 color, GLfloat rotate,
    GLfloat speed, GLfloat opacity, GLuint life)
    : shape(shape),
    position(position), scale(scale), direction(glm::normalize(direction)),
    color(color), rotate(rotate),
    speed(speed), opacity(opacity), life(life), VBO(0), VAO(0) {

    float quad[] = {
        -.5f,  .5f, 0.f, 0.f, 1.f,
        .5f,  .5f, 0.f, 1.f, 1.f,
        .5f, -.5f, 0.f, 1.f, 0.f,

        -.5f,  .5f, 0.f, 0.f, 1.f,
        -.5f, -.5f, 0.f, 0.f, 0.f,
        .5f, -.5f, 0.f, 1.f, 0.f,
    };
    float cube[] = {
       -0.5f, -0.5f, -0.5f,  0.f, 0.f,
        0.5f, -0.5f, -0.5f,  1.f, 0.f,
        0.5f,  0.5f, -0.5f,  1.f, 1.f,
        0.5f,  0.5f, -0.5f,  1.f, 1.f,
       -0.5f,  0.5f, -0.5f,  0.f, 1.f,
       -0.5f, -0.5f, -0.5f,  0.f, 0.f,

       -0.5f, -0.5f,  0.5f,  0.f, 0.f,
        0.5f, -0.5f,  0.5f,  1.f, 0.f,
        0.5f,  0.5f,  0.5f,  1.f, 1.f,
        0.5f,  0.5f,  0.5f,  1.f, 1.f,
       -0.5f,  0.5f,  0.5f,  0.f, 1.f,
       -0.5f, -0.5f,  0.5f,  0.f, 0.f,

       -0.5f,  0.5f,  0.5f,  1.f, 0.f,
       -0.5f,  0.5f, -0.5f,  1.f, 1.f,
       -0.5f, -0.5f, -0.5f,  0.f, 1.f,
       -0.5f, -0.5f, -0.5f,  0.f, 1.f,
       -0.5f, -0.5f,  0.5f,  0.f, 0.f,
       -0.5f,  0.5f,  0.5f,  1.f, 0.f,

        0.5f,  0.5f,  0.5f,  1.f, 0.f,
        0.5f,  0.5f, -0.5f,  1.f, 1.f,
        0.5f, -0.5f, -0.5f,  0.f, 1.f,
        0.5f, -0.5f, -0.5f,  0.f, 1.f,
        0.5f, -0.5f,  0.5f,  0.f, 0.f,
        0.5f,  0.5f,  0.5f,  1.f, 0.f,

       -0.5f, -0.5f, -0.5f,  0.f, 1.f,
        0.5f, -0.5f, -0.5f,  1.f, 1.f,
        0.5f, -0.5f,  0.5f,  1.f, 0.f,
        0.5f, -0.5f,  0.5f,  1.f, 0.f,
       -0.5f, -0.5f,  0.5f,  0.f, 0.f,
       -0.5f, -0.5f, -0.5f,  0.f, 1.f,

       -0.5f,  0.5f, -0.5f,  0.f, 1.f,
        0.5f,  0.5f, -0.5f,  1.f, 1.f,
        0.5f,  0.5f,  0.5f,  1.f, 0.f,
        0.5f,  0.5f,  0.5f,  1.f, 0.f,
       -0.5f,  0.5f,  0.5f,  0.f, 0.f,
       -0.5f,  0.5f, -0.5f,  0.f, 1.f
    };
    float *vertices = NULL;
    if (shape == PARTICLE_SHAPE_QUAD) {
        numVertices = 6;
        vertices = quad;
    } else if (shape == PARTICLE_SHAPE_CUBE) {
        numVertices = 36;
        vertices = cube;
    }
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 5,
        vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
        (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

}

Particle::~Particle() {

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

}

void Particle::bind() {

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);

}

void Particle::move() {

    position += direction * speed;
    life--;

}

bool Particle::dead() {

    return life == 0;

}

glm::mat4 Particle::getModel() const {

    glm::mat4 model(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.f, 1.f, 0.f));
    return model;

}

glm::vec3 Particle::getColor() const {
    return color;
}

GLfloat Particle::getOpacity() const {
    return opacity;
}

ParticleRenderer::ParticleRenderer(Shader *shader, Texture *texture,
    const GLchar *config) : texture(texture), Renderer(shader) {

    FILE *configFile = fopen(config, "r");
    if (!configFile) {
        printf("\n[ERROR]In ParticleRenderer\n");
        printf("%s not found\n", config);
        return;
    }
    fseek(configFile, 0, SEEK_END);
    long n = ftell(configFile);
    fseek(configFile, 0, SEEK_SET);
    GLchar *buf = new GLchar[n];
    fread(buf, 1, n, configFile);
    fclose(configFile);

    cJSON *parameters = cJSON_Parse(buf);
    type = (ParticleEmitterType)cJSON_GetObjectItem(parameters,
        "emitterType")->valueint;
    shape = (ParticleShape)cJSON_GetObjectItem(parameters, "shape")->valueint;
    life = cJSON_GetObjectItem(parameters, "life")->valueint;
    count = cJSON_GetObjectItem(parameters, "count")->valueint;
    position = parseVec3(cJSON_GetObjectItem(parameters, "position")->valuestring);
    scale = parseVec3(cJSON_GetObjectItem(parameters, "scale")->valuestring);
    direction = parseVec3(cJSON_GetObjectItem(parameters, "direction")->valuestring);
    rotate = (GLfloat)cJSON_GetObjectItem(parameters, "rotate")->valuedouble;
    speed = (GLfloat)cJSON_GetObjectItem(parameters, "speed")->valuedouble;
    opacity = (GLfloat)cJSON_GetObjectItem(parameters, "opacity")->valuedouble;
    color = parseVec3(cJSON_GetObjectItem(parameters, "color")->valuestring);
    randPos = parseVec3(cJSON_GetObjectItem(parameters, "randPos")->valuestring);
    randScale = parseVec3(cJSON_GetObjectItem(parameters, "randScale")->valuestring);
    randDir = parseVec3(cJSON_GetObjectItem(parameters, "randDir")->valuestring);
    randRotate = (GLfloat)cJSON_GetObjectItem(parameters, "randRotate")->valuedouble;
    randSpeed = (GLfloat)cJSON_GetObjectItem(parameters, "randSpeed")->valuedouble;
    randOpacity = (GLfloat)cJSON_GetObjectItem(parameters, "randOpacity")->valuedouble;
    randLife = (GLuint)cJSON_GetObjectItem(parameters, "randLife")->valueint;
    delete []buf;
    cJSON_Delete(parameters);

    initRenderData();

}

ParticleRenderer::~ParticleRenderer() {

    if (particles != NULL) {
        for (GLuint i = 0; i < particles->size(); i++) {
            delete particles->at(i);
        }
        delete particles;
    }

}

void ParticleRenderer::initRenderData() {

    particles = new std::vector<Particle *>;
    for (GLuint i = 0; i < count; i++) {
        particles->push_back(generate());
    }

}

void ParticleRenderer::enable() {

    shader->use();
    texture->bind();

}

void ParticleRenderer::draw(glm::mat4 projection, glm::mat4 view,
    glm::vec3 pos, glm::vec3 scale, glm::vec3 rotAxis, GLfloat rotate, glm::vec3 viewPos) {

    enable();
    for (GLuint i = 0; i < particles->size(); i++) {
        Particle *particle = particles->at(i);
        shader->uniformM4("projection", projection);
        shader->uniformM4("view", view);
        shader->uniformM4("model", particle->getModel());
        shader->uniform4("color", glm::vec4(particle->getColor(),
            particle->getOpacity()));
        shader->uniform1("texture0", 0);
        particle->bind();
        GLuint numVertices = 0;
        switch (shape) {
        case PARTICLE_SHAPE_CUBE:
            numVertices = 36;
            break;
        case PARTICLE_SHAPE_QUAD:
            numVertices = 6;
            break;
        }
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        particle->move();
    }

    GLuint numErased = 0;
    for (std::vector<Particle *>::iterator it = particles->begin();
        it != particles->end();) {
        if ((*it)->dead()) {
            Particle *particle = *it;
            it = particles->erase(it);
            delete particle;
            numErased++;
        } else {
            it++;
        }
    }
    for (GLuint i = 0; i < numErased; i++) {
        particles->push_back(generate());
    }

}

Particle *ParticleRenderer::generate() {

    glm::vec3 pos = position, sca = scale, dir = direction;
    GLfloat rot = rotate, spd = speed, opa = opacity;
    GLuint l = life;
    if (type == PARTICLE_BOX) {
        pos += glm::vec3(randf(randPos.x), randf(randPos.y), randf(randPos.z));
    }
    sca += glm::vec3(randf(randScale.x), randf(randScale.y), randf(randScale.z));
    dir += glm::vec3(randf(randDir.x), randf(randDir.y), randf(randDir.z));
    rot += randf(randRotate);
    spd += randf(randSpeed);
    opa += randf(randOpacity);
    l += (GLint)randf((GLfloat)randLife);

    Particle *particle = new Particle(shape, pos, sca, dir, color, rot,
        spd, opa, l);
    return particle;

}