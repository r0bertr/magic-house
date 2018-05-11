#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/glad.h"
#include "ResourceManager.hpp"

#include <GLFW/glfw3.h>

char *readAll(FILE *file) {
    char *result = new char[4096];
    result[0] = '\0';
    char curLine[1024];
    while (fgets(curLine, sizeof(curLine), file) != NULL) {
        strcat(result, curLine);
    }
    return result;
}

ResourceManager *ResourceManager::GetInstance() {
    static ResourceManager *instance;
    if (instance == NULL) {
        instance = new ResourceManager();
    }
    return instance;
}

Camera *ResourceManager::loadCamera(GLfloat pFov, GLfloat pAspect,
    GLfloat pNear, GLfloat pFar, GLfloat posX, GLfloat posY, GLfloat posZ,
    GLfloat frontX, GLfloat frontY, GLfloat frontZ,
    GLfloat upX, GLfloat upY, GLfloat upZ, GLchar *name) {

    if (!cameras[name]) {
        delete cameras[name];
    }
    cameras[name] = new Camera(pFov, pAspect, pNear, pFar, posX, posY,
        posZ, frontX, frontY, frontZ, upX, upY, upZ);

    return cameras[name];
}

Shader *ResourceManager::loadShader(const GLchar *vPath, const GLchar *fPath,
    const GLchar *gPath, GLchar *name) {

    GLchar *vCode = NULL, *fCode = NULL, *gCode = NULL;
    FILE *vFile = NULL, *fFile = NULL, *gFile = NULL;
    if (vPath) {
        vFile = fopen(vPath, "r");
        vCode = readAll(vFile);
    }
    if (fPath) {
        fFile = fopen(fPath, "r");
        fCode = readAll(fFile);
    }
    if (gPath) {
        gFile = fopen(gPath, "r");
        gCode = readAll(gFile);
    }

    GLint success;
    GLchar infoLog[512];

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vCode, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        printf("[ERROR]In ResourceManager::loadShader\n%s\n", infoLog);
        return NULL;
    }

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fCode, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        printf("[ERROR]In ResourceManager::loadShader\n%s\n", infoLog);
        return NULL;
    }

    // TODO: Compile gCode here...

    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("[ERROR]In ResourceManager::loadShader\n%s\n", infoLog);
        return NULL;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    // glDeleteShader(gShader);

    if (shaders[name] != NULL) {
        delete shaders[name];
    }
    shaders[name] = new Shader(program);

    return shaders[name];

}

Texture *ResourceManager::load2DTexture(const GLchar *srcPath, GLchar *name) {

    stbi_set_flip_vertically_on_load(true);
    GLint width, height, nChannels;
    GLubyte *data = stbi_load(srcPath, &width, &height, &nChannels, 0);
    GLenum colorModel = GL_RGB;
    if (!data) {
        printf("[ERROR]In ResourceManager::load2DTexture\n");
        return NULL;
    }
    if (nChannels == 4)
        colorModel = GL_RGBA;
    if (!textures[name]) {
        delete textures[name];
    }
    textures[name] = new Texture(GL_TEXTURE_2D, data, colorModel, width, height);
    stbi_image_free(data);
    return textures[name];

}

SpriteRenderer *ResourceManager::loadRenderer(const SpriteType type,
    Shader *shader, GLchar *name) {
    
    if (!renderers[name])
        delete renderers[name];
    switch (type) {
    case SPRITE_CUBE:
        renderers[name] = new CubeRenderer(shader);
        break;
    default:
        printf("[ERROR]In ResourceManager::loadRenderer\nWrong type\n");
        return NULL;
    }

    return renderers[name];
}

Shader *ResourceManager::getShader(GLchar *name) {
    return shaders[name];
}

Texture *ResourceManager::getTexture(GLchar *name) {
    return textures[name];
}

SpriteRenderer *ResourceManager::getRenderer(GLchar *name) {
    return renderers[name];
}

Camera *ResourceManager::getCamera(GLchar *name) {
    return cameras[name];
}

ResourceManager::ResourceManager() {}