#include "Frog.hpp"

MyFrog::MyFrog(glm::vec3 tempPos, glm::vec3 tempScale, glm::vec3 tempRotAxis, GLfloat tempRotate) {
    frog_position = tempPos;
    scale         = tempScale;
    rotAxis       = tempRotAxis;
    rotate        = tempRotate;
}

void MyFrog::changeWeather() {

}

void MyFrog::changeTimeSpeed() {
    
}

glm::vec3 MyFrog::getPosition() {
    return frog_position;
}

glm::vec3 MyFrog::getScale() {
    return scale;
}

glm::vec3 MyFrog::getRotAxis() {
    return rotAxis;
}

GLfloat MyFrog::getRotate() {
    return rotate;
}