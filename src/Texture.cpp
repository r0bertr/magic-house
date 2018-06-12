#include "lib/glad.h"
#include "Texture.hpp"

Texture::Texture(GLenum type, GLubyte *data, GLenum colorModel,
    GLint width, GLint height) {
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(type, id);
    glTexImage2D(type, 0, colorModel, width, height, 0, colorModel,
        GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(type);
    this->type = type;
}

Texture::Texture(GLenum type, GLint SHADOW_WIDTH, GLint SHADOW_HEIGHT) {
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(type, id);
	glTexImage2D(type, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
        0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	this->type = type;
}

Texture::Texture(GLenum type, GLubyte *data[6], GLenum internalFormats[6],
	GLenum dataFormats[6], GLint width[6], GLint height[6]) {
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(type, id);
    for (GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormats[i],
            width[i], height[i], 0, dataFormats[i], GL_UNSIGNED_BYTE, data[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    this->type = type;
}

void Texture::bind(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, id);
}

GLuint Texture::getID() const {
    return id;
}