#include "lib/glad.h"
#include "Shader.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

Shader::Shader(const GLuint id) {
    this->id = id;
}

void Shader::use() {
    glUseProgram(id);
}

GLuint Shader::getID() const {
    return id;
}

void Shader::uniform1(const char *name, const GLfloat val) {
    glUniform1f(glGetUniformLocation(id, name), val);
}

void Shader::uniform1(const char *name, const GLuint val) {
    glUniform1i(glGetUniformLocation(id, name), val);
}

void Shader::uniform1(const char *name, const GLint val) {
    glUniform1i(glGetUniformLocation(id, name), val);
}

void Shader::uniform3(const char *name, const GLfloat x,
    const GLfloat y, const GLfloat z) {
    glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void Shader::uniform3(const char *name, const GLfloat vec[3]) {
    glUniform3fv(glGetUniformLocation(id, name), 1, vec);
}

void Shader::uniform3(const char *name, const glm::vec3 vec) {
    glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(vec));
}

void Shader::uniformM4(const char *name, const glm::mat4 mat) {
    GLint loc = glGetUniformLocation(id, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

