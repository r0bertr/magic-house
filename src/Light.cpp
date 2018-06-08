#define _USE_MATH_DEFINES
#include "Light.hpp"

#include <cstdio>
#include <math.h>

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

// house position as center
void Light::rotate(glm::vec3 center) {
	float time = glfwGetTime();
	float maxR = 500.f;
	float minR = 350.f;
	int period = 30; // 30seconds

	position.x = center.x;
	position.y = minR * sin(2 * M_PI*time / period);
	position.z = maxR * cos(2 * M_PI*time / period);

	direction = center - position;

	if (position.y >= 0) {
		diffuse = 0.7*sin(2 * M_PI*time / period) + 0.3f;
		specular = 0.7*sin(2 * M_PI*time / period) + 0.3f;
		color = glm::vec3(0.7*sin(2 * M_PI*time / period) + 0.3f);
	}
	else {
		diffuse = 0.2*sin(2 * M_PI*time / period) + 0.3f;
		specular = 0.2*sin(2 * M_PI*time / period) + 0.3f;
		color = glm::vec3(0.2*sin(2 * M_PI*time / period) + 0.3f);
	}
}

