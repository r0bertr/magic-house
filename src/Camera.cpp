#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(GLfloat pFov, GLfloat pAspect, GLfloat pNear, GLfloat pFar,
        GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat frontX, GLfloat frontY, GLfloat frontZ,
        GLfloat upX, GLfloat upY, GLfloat upZ) {

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

void Camera::moveForward(const GLfloat &distance) {
    pos += distance * front;
}

void Camera::moveBack(const GLfloat &distance) {
    pos -= distance * front;
}

void Camera::moveLeft(const GLfloat &distance) {
    pos -= distance * right;
}

void Camera::moveRight(const GLfloat &distance) {
    pos += distance * right;
}

void Camera::moveForward2D(const GLfloat &distance) {
    pos += distance * glm::vec3(front.x, 0.f, front.z);
}

void Camera::moveBack2D(const GLfloat &distance) {
    pos -= distance * glm::vec3(front.x, 0.f, front.z);
}

void Camera::moveLeft2D(const GLfloat &distance) {
    pos -= distance * glm::vec3(right.x, 0.f, right.z);
}

void Camera::moveRight2D(const GLfloat &distance) {
    pos += distance * glm::vec3(right.x, 0.f, right.z);
}

void Camera::rotateLeft(const GLfloat &quantity) {
    yaw -= quantity;
    rotate(pitch, yaw);
}

void Camera::rotateRight(const GLfloat &quantity) {
    yaw += quantity;
    rotate(pitch, yaw);
}

void Camera::rotateUp(const GLfloat &quantity) {
    pitch += quantity;
    pitch = fmin(pitch, 3.14 / 2);
    rotate(pitch, yaw);
}

void Camera::rotateDown(const GLfloat &quantity) {
    pitch -= quantity;
    pitch = fmax(pitch, -3.14 / 2);
    rotate(pitch, yaw);
}

void Camera::rotate(const GLfloat pitch, const GLfloat yaw) {
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