#include "Light.hpp"

Light::Light(
    LightType type,
    glm::vec3 position,
    glm::vec3 color,
    glm::vec3 direction,
    GLfloat ambient,
    GLfloat diffuse,
    GLfloat specular
) :
    type(type),
    position(position),
    color(color),
    direction(direction),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular) {}

Light::~Light() {}

glm::vec3 Light::getPosition() const {
    return position;
}

glm::vec3 Light::getColor() const {
    return color;
}

glm::vec3 Light::getDirection() const {
    return direction;
}

GLfloat Light::getAmbient() const {
    return ambient;
}

GLfloat Light::getDiffuse() const {
    return diffuse;
}

GLfloat Light::getSpecular() const {
    return specular;
}
