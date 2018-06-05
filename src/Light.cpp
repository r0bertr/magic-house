#include "Light.hpp"

#include <cstdio>

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
    specular(specular) {

    if (type == LIGHT_POINT) {
        direction = glm::vec3(0.f);
    }

}

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
