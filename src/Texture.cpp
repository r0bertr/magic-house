#include "lib/glad.h"
#include "Texture.hpp"

Texture::Texture(GLenum type, GLubyte *data, GLenum colorModel,
    GLint width, GLint height) {
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(type, id);
    if (type == GL_TEXTURE_2D) {
        glTexImage2D(type, 0, colorModel, width, height, 0, colorModel,
            GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(type);
}

void Texture::bind(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, id);
}

GLuint Texture::getID() const {
    return id;
}