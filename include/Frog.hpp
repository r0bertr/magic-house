#pragma once

#include "lib/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

class MyFrog {
private:
    glm::vec3 frog_position;
    glm::vec3 scale;
    glm::vec3 rotAxis;
    GLfloat   rotate;

    GLfloat timeSpeed;
    GLint   weather;
    glm::vec3 sunRotateSpeed;

    bool ifRender;
    // Text renderer variable;
    unsigned int Text2DTextureID;
    unsigned int Text2DVertexBufferID;
    unsigned int Text2DUVBufferID;
    unsigned int Text2DShaderID;
    unsigned int Text2DUniformID;

    GLuint loadDDS(const char * imagepath);

public:
	const static GLfloat distance;

    MyFrog(const char *texturePath,
    glm::vec3 tempPos = glm::vec3(15.f, 0.5f, 10.f),
    glm::vec3 tempScale    = glm::vec3(3.f),
    glm::vec3 tempRotAxis  = glm::vec3(1.0f, 0.f, 0.f),
    GLfloat tempRotate     = 215.0f);
    ~MyFrog();

    void changeWeather();
    void changeTimeSpeed(GLfloat speed);
    void setIfRender(bool flag);
    // 加减速时间
    void speedUp();
    void slowDown();

    glm::vec3 getPosition();
    glm::vec3 getScale();
    glm::vec3 getRotAxis();
    glm::vec3 getSunRotSpeed();
    GLfloat   getRotate();
    bool      getIfRender();

    void printText2D(const char * text, int x, int y, int size);
    void cleanupText2D();
};