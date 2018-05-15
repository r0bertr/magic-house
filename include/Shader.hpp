#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Shader {
public:
    Shader(const GLuint id);

    void use();
    void uniform1(const char *name, const GLfloat val);
    void uniform1(const char *name, const GLint val);
    void uniform1(const char *name, const GLuint val);
    void uniform3(const char *name, const GLfloat x,
        const GLfloat y, const GLfloat z);
    void uniform3(const char *name, const GLfloat vec[3]);
    void uniform3(const char *name, const glm::vec3 vec);
    void uniformM4(const char *name, const glm::mat4 mat);

    GLuint getID() const;

private:
    GLuint id;
};
