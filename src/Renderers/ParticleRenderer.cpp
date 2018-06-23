#include "Renderers/ParticleRenderer.hpp"
#include "ResourceManager.hpp"

template<class T>
RandValue<T>::RandValue() {}

template<class T>
RandValue<T>::RandValue(T m, T v) {
    mean = m;
    variance = v;
}

template<>
glm::vec3 RandValue<glm::vec3>::eval() const {

    return glm::vec3(
        mean.x  + ((GLfloat)rand() / RAND_MAX - 0.5f) * variance.x,
        mean.y  + ((GLfloat)rand() / RAND_MAX - 0.5f) * variance.y,
        mean.z  + ((GLfloat)rand() / RAND_MAX - 0.5f) * variance.z
    );

}

template<class T>
T RandValue<T>::eval() const {

    return mean + ((GLfloat)rand() / RAND_MAX - 0.5f) * variance;

}

Particle::Particle(const ParticleRenderer &r) :
    ParticleRenderer(r) {
    
    model = NULL;

    position = attributes->initPosition.mean;
    direction = attributes->initDirection.mean;
    speed = attributes->initSpeed.mean;
    color = attributes->initColor.mean;
    size = attributes->initSize.mean;
    life = attributes->initLife;

    initRenderData();

}

ParticleAttributes *ParticleRenderer::getAttributes() {

    return attributes;

}

ParticleAttributes::ParticleAttributes(const GLchar *path) {

    FILE *configFile = fopen(path, "r");
    if (!configFile) {
        printf("\n[ERROR]In ParticleAttributes\n");
        printf("%s not found\n", path);
        return;
    }
    fseek(configFile, 0, SEEK_END);
    long n = ftell(configFile);
    fseek(configFile, 0, SEEK_SET);
    GLchar *buf = new GLchar[n];
    fread(buf, 1, n, configFile);
    fclose(configFile);

    cJSON *parameters = cJSON_Parse(buf);
    setAttributes(parameters);
    delete []buf;
    cJSON_Delete(parameters);

}

Particle::~Particle() {}

void Particle::draw(
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

    if (model) {
        model->draw(
            projection,
            view,
            viewPos,
            pos + position,
            scale + size,
            rotAxis,
            rotate,
            this->color,
            collisionSize
        );
    } else {
        Renderer::draw(
            projection,
            view,
            viewPos,
            pos + position,
            scale + size,
            rotAxis,
            rotate,
            this->color,
            collisionSize
        );
    }
    animate();

}

bool Particle::dead() {
    return life == 0;
}

void Particle::animate() {

    // movement
    GLfloat t = (GLfloat)(attributes->initLife - life);
    glm::vec3 s1 = t * glm::normalize(direction) * speed;
    if (attributes->gravity) {
        glm::vec3 s2 = 0.5f * glm::normalize(attributes->gravityDir) * 
            attributes->gravityMag * t * t;
        position = attributes->initPosition.mean + s1 + s2;
    } else {
        position = attributes->initPosition.mean + s1;
    }

    // life
    life--;

}

void Particle::enable() {

    if (!model) {
        Renderer::enable();
    }

}

void Particle::initRenderData() {

    if (attributes->initShape == PARTICLE_SHAPE_SPHERE) {
        model = ResourceManager::GetInstance()->loadModel(
            "res/models/sphere/sphere.obj",
            shader, light, "particle.sphere");
    } else if (attributes->initShape == PARTICLE_SHAPE_RECT) {
        GLfloat vertices[] = {
            -.5f, 0.f, -.5f, 0.f, 0.f, 0.f, 1.f, 0.f,
            -.5f, 0.f,  .5f, 0.f, 1.f, 0.f, 1.f, 0.f,
             .5f, 0.f,  .5f, 1.f, 1.f, 0.f, 1.f, 0.f,
            -.5f, 0.f, -.5f, 0.f, 0.f, 0.f, 1.f, 0.f,
             .5f, 0.f, -.5f, 1.f, 0.f, 0.f, 1.f, 0.f,
             .5f, 0.f,  .5f, 1.f, 1.f, 0.f, 1.f, 0.f,
        };
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
            GL_STATIC_DRAW);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat), NULL);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

}

ParticleRenderer::ParticleRenderer(Shader *shader, const GLchar *config,
    Light *light) :
    Renderer(shader, NULL, light) {

    attributes = new ParticleAttributes(config);

    initRenderData();

}

void ParticleRenderer::setShader(Shader *shader) {

    for (GLuint i = 0; i < children.size(); i++) {
        children[i]->setShader(shader);
    }

}

ParticleAttributes::ParticleAttributes() {}

ParticleRenderer::ParticleRenderer(const ParticleRenderer &r) :
    Renderer(r.shader, NULL, r.light) {
    
    attributes = new ParticleAttributes();
    attributes->numParticles.mean = r.attributes->numParticles.eval();
    attributes->numParticles.variance = r.attributes->numParticles.variance;
    attributes->initPosition.mean = r.attributes->initPosition.eval();
    attributes->initPosition.variance = r.attributes->initPosition.variance;
    attributes->initDirection.mean = r.attributes->initDirection.eval();
    attributes->initDirection.variance = r.attributes->initDirection.variance;
    attributes->initSpeed.mean = r.attributes->initSpeed.eval();
    attributes->initSpeed.variance = r.attributes->initSpeed.variance;
    attributes->initSize.mean = r.attributes->initSize.eval();
    attributes->initSize.variance = r.attributes->initSize.variance;
    attributes->initColor.mean = r.attributes->initColor.eval();
    attributes->initColor.variance = r.attributes->initColor.variance;
    attributes->initShape = r.attributes->initShape;
    attributes->initLife = r.attributes->initLife;
    attributes->gravity = r.attributes->gravity;
    attributes->gravityDir = r.attributes->gravityDir;
    attributes->gravityMag = r.attributes->gravityMag;
    initRenderData();

}

ParticleRenderer::~ParticleRenderer() {
    for (GLuint i = 0; i < children.size(); i++){
        delete children[i];
    }
}

void ParticleRenderer::draw(
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

    spawn();
    for (GLuint i = 0; i < children.size(); i++) {
        children[i]->draw(projection, view, viewPos, pos, scale, rotAxis,
            rotate, color, collisionSize);
    }
    for (std::vector<ParticleRenderer *>::iterator iter = children.begin();
        iter != children.end();) {
        if ((*iter)->dead()) {
            iter = children.erase(iter);
        } else {
            iter++;
        }
    }

}

bool ParticleRenderer::dead() {
    return false;
}

void ParticleRenderer::spawn() {

    GLuint num = attributes->numParticles.eval();
    for (GLuint i = 0; i < num; i++) {
        ParticleRenderer *r = NULL;
        r = new Particle(*this);
        children.push_back(r);
    }

}

void ParticleRenderer::enable() {}

void ParticleRenderer::initRenderData() {}

void ParticleAttributes::setAttributes(cJSON *parameters) {

    numParticles.mean = cJSON_GetObjectItem(parameters,
        "num_particles_mean")->valueint;
    numParticles.variance = cJSON_GetObjectItem(parameters,
        "num_particles_variance")->valueint;
    initPosition.mean.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_position_mean_x")->valuedouble;
    initPosition.mean.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_position_mean_y")->valuedouble;
    initPosition.mean.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_position_mean_z")->valuedouble;
    initPosition.variance.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_position_variance_x")->valuedouble;
    initPosition.variance.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_position_variance_y")->valuedouble;
    initPosition.variance.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_position_variance_z")->valuedouble;
    initDirection.mean.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_direction_mean_x")->valuedouble;
    initDirection.mean.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_direction_mean_y")->valuedouble;
    initDirection.mean.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_direction_mean_z")->valuedouble;
    initDirection.variance.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_direction_variance_x")->valuedouble;
    initDirection.variance.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_direction_variance_y")->valuedouble;
    initDirection.variance.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_direction_variance_z")->valuedouble;
    initSpeed.mean = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_speed_mean")->valuedouble;
    initSpeed.variance = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_speed_variance")->valuedouble;
    initSize.mean.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_size_mean_x")->valuedouble;
    initSize.mean.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_size_mean_y")->valuedouble;
    initSize.mean.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_size_mean_z")->valuedouble;
    initSize.variance.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_size_variance_x")->valuedouble;
    initSize.variance.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_size_variance_y")->valuedouble;
    initSize.variance.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_size_variance_z")->valuedouble;
    initColor.mean.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_mean_x")->valuedouble;
    initColor.mean.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_mean_y")->valuedouble;
    initColor.mean.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_mean_z")->valuedouble;
    initColor.mean.w = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_mean_w")->valuedouble;
    initColor.variance.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_variance_x")->valuedouble;
    initColor.variance.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_variance_y")->valuedouble;
    initColor.variance.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_variance_z")->valuedouble;
    initColor.variance.w = (GLfloat)cJSON_GetObjectItem(parameters,
        "init_color_variance_w")->valuedouble;
    initShape = (ParticleShape)cJSON_GetObjectItem(parameters,
        "init_shape")->valueint;
    initLife = (GLuint)cJSON_GetObjectItem(parameters,
        "init_life")->valueint;
    gravity = (bool)cJSON_GetObjectItem(parameters,
        "gravity")->valueint;
    gravityDir.x = (GLfloat)cJSON_GetObjectItem(parameters,
        "gravity_dir_x")->valuedouble;
    gravityDir.y = (GLfloat)cJSON_GetObjectItem(parameters,
        "gravity_dir_y")->valuedouble;
    gravityDir.z = (GLfloat)cJSON_GetObjectItem(parameters,
        "gravity_dir_z")->valuedouble;
    gravityMag = (GLfloat)cJSON_GetObjectItem(parameters,
        "gravity_mag")->valuedouble;

}