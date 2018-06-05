#pragma once

#include <GLFW/glfw3.h>

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
        GLfloat ambient,
        GLfloat diffuse,
        GLfloat specular
    );
    ~Light();

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    glm::vec3 getDirection() const;
    GLfloat getAmbient() const;
    GLfloat getDiffuse() const;
    GLfloat getSpecular() const;
private:
    LightType type;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 direction;
    GLfloat ambient;
    GLfloat diffuse;
    GLfloat specular;
};