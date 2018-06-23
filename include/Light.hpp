#pragma once

#include "lib/glad.h"

#include <glm/glm.hpp>

enum LightType {
    LIGHT_POINT,
    LIGHT_DIRECT,
};

class Light {
public:
    Light(
        LightType type,
        glm::vec3 position,
        glm::vec3 color,
        glm::vec3 direction,
		glm::mat4 projection,
        GLfloat ambient,
        GLfloat diffuse,
        GLfloat specular
    );
    ~Light();

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    glm::vec3 getDirection() const;
	glm::mat4 getProjection() const;
    GLfloat getAmbient() const;
    GLfloat getDiffuse() const;
    GLfloat getSpecular() const;
	float rotate(glm::vec3 center);

private:
    LightType type;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 direction;
	glm::mat4 projection;
    GLfloat ambient;
    GLfloat diffuse;
    GLfloat specular;
};