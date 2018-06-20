#pragma once

#include <glm/glm.hpp>
#include "lib/glad.h"
#include <GLFW/glfw3.h>

class MyFrog {
private:
    glm::vec3 frog_position;
    glm::vec3 scale;
    glm::vec3 rotAxis;
    GLfloat   rotate;

    GLfloat timeSpeed;
    GLint   weather;

public:
    const static GLfloat distance = 5.0f;

    MyFrog(glm::vec3 tempPos = glm::vec3(15.f, 0.5f, 10.f),
    glm::vec3 tempScale    = glm::vec3(3.f),
    glm::vec3 tempRotAxis  = glm::vec3(1.0f, 0.f, 0.f),
    GLfloat tempRotate     = 215.0f);

    void changeWeather();
    void changeTimeSpeed();

    glm::vec3 getPosition();
    glm::vec3 getScale();
    glm::vec3 getRotAxis();
    GLfloat   getRotate();
};