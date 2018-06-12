#pragma once

#include <GLFW/glfw3.h>

class Texture {
public:
    Texture(GLenum type, GLubyte *data, GLenum internalFormat,
        GLenum dataFormat, GLint width, GLint height);
	Texture(GLenum type, GLint SHADOW_WIDTH, GLint SHADOW_HEIGHT);
    Texture(GLenum type, GLubyte *data[6], GLenum internalFormats[6],
		GLenum dataFormats[6], GLint width[6], GLint height[6]);

    void bind(GLuint unit = 0);

    GLuint getID() const;
private:
    GLenum type;
    GLuint id;
};
