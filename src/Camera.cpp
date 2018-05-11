#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float pFov, float pAspect, float pNear, float pFar,
        float posX, float posY, float posZ,
        float frontX, float frontY, float frontZ,
        float upX, float upY, float upZ) {

    this->pFov = pFov;
    this->pAspect = pAspect;
    this->pNear = pNear;
    this->pFar = pFar;
    this->pos = glm::vec3(posX, posY, posZ);
    this->front = glm::normalize(glm::vec3(frontX, frontY, frontZ));
    this->up = glm::vec3(upX, upY, upZ);
    this->right = glm::normalize(glm::cross(this->front, this->up));

    pitch = asin(front.y);
    yaw = atan(front.z / front.x);
}

glm::mat4 Camera::getProjection() const {
    return glm::perspective(pFov, pAspect, pNear, pFar);
}

void Camera::moveForward(const float &distance) {
    pos += distance * front;
}

void Camera::moveBack(const float &distance) {
    pos -= distance * front;
}

void Camera::moveLeft(const float &distance) {
    pos -= distance * right;
}

void Camera::moveRight(const float &distance) {
    pos += distance * right;
}

void Camera::rotateLeft(const float &quantity) {
    yaw -= quantity;
    rotate(pitch, yaw);
}

void Camera::rotateRight(const float &quantity) {
    yaw += quantity;
    rotate(pitch, yaw);
}

void Camera::rotateUp(const float &quantity) {
    pitch += quantity;
    pitch = fmin(pitch, 3.14 / 2);
    rotate(pitch, yaw);
}

void Camera::rotateDown(const float &quantity) {
    pitch -= quantity;
    pitch = fmax(pitch, -3.14 / 2);
    rotate(pitch, yaw);
}

void Camera::rotate(const float pitch, const float yaw) {
    front.x = cos(pitch) * cos(yaw);
    front.y = sin(pitch);
    front.z = cos(pitch) * sin(yaw);
    right = glm::normalize(glm::cross(front, up));
}

glm::vec3 Camera::getPos() const {
    return pos;
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(pos, pos + front, up);
}