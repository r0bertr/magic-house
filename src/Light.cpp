#define _USE_MATH_DEFINES

#include "Light.hpp"

#include <GLFW/glfw3.h>

Light::Light(
    LightType type,
    glm::vec3 position,
    glm::vec3 color,
    glm::vec3 direction,
	glm::mat4 projection,
    GLfloat ambient,
    GLfloat diffuse,
    GLfloat specular
) :
    type(type),
    position(position),
    color(color),
    direction(direction),
	projection(projection),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular) {

    if (type == LIGHT_POINT) {
        this->direction = glm::vec3(0.f);
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

glm::mat4 Light::getProjection() const {
	return projection;
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
float Light::rotate(glm::vec3 center) {
	float time = glfwGetTime();
	float maxR = 500.f;
	float minR = 350.f;
	float alpha;
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
	
	alpha = 0.5*sin(2 * M_PI*time / period) + 0.5f;
	return alpha;
}

