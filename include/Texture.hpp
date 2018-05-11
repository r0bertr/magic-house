#pragma once

#include <GLFW/glfw3.h>

class Texture {
public:
    Texture(GLenum type, GLubyte *data, GLenum colorModel,
        GLint width, GLint height);

    void bind();

    GLuint getID() const;
private:
    GLenum type;
    GLuint id;
};