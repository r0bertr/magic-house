#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/glad.h"
#include "ResourceManager.hpp"

#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define BUFFER_LEN 32

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
    GLfloat upX, GLfloat upY, GLfloat upZ, const GLchar *name) {

    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (cameras[n]) {
        return cameras[n];
    }
    cameras[n] = new Camera(pFov, pAspect, pNear, pFar, posX, posY,
        posZ, frontX, frontY, frontZ, upX, upY, upZ);

    return cameras[n];
}

Shader *ResourceManager::loadShader(const GLchar *vPath, const GLchar *fPath,
    const GLchar *gPath, const GLchar *name) {

    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (shaders[n]) {
        return shaders[n];
    }

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
    GLchar infoLog[256];

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vCode, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader, 256, NULL, infoLog);
        printf("[ERROR]In ResourceManager::loadShader\n\t%s\n", infoLog);
        return NULL;
    }

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fCode, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, 256, NULL, infoLog);
        printf("[ERROR]In ResourceManager::loadShader\n\t%s\n", infoLog);
        return NULL;
    }

    // TODO: Compile gCode here...

    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 256, NULL, infoLog);
        printf("[ERROR]In ResourceManager::loadShader\n%s\n", infoLog);
        return NULL;
    }

    delete []vCode;
    delete []fCode;
    // delete []gCode;
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    // glDeleteShader(gShader);

    shaders[n] = new Shader(program);
    return shaders[n];

}

Texture *ResourceManager::load2DTexture(const GLchar *srcPath, const GLchar *name) {
    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (textures[n]) {
        return textures[n];
    }

    GLint width, height, nChannels;
    GLubyte *data = stbi_load(srcPath, &width, &height, &nChannels, 0);
    GLenum colorModel = GL_RGB;
    if (!data) {
        printf("[ERROR]In ResourceManager::load2DTexture\n");
        printf("\tError loading file\n");
        return NULL;
    }

    if (nChannels == 4)
        colorModel = GL_RGBA;
    textures[n] = new Texture(GL_TEXTURE_2D, data, colorModel, width, height);
    stbi_image_free(data);
    return textures[n];

}

Renderer *ResourceManager::loadParticleRenderer(Shader *shader,
    Texture *texture, const GLchar *config, const GLchar *name) {

    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (renderers[n]) {
        return renderers[n];
    }

    Renderer *renderer = new ParticleRenderer(shader, texture, config);
    renderers[n] = renderer;
    return renderer;

}

Texture *ResourceManager::loadBoxTexture(const GLchar *srcPath[6],
    const GLchar *name) {
    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (textures[n]) {
        return textures[n];
    }

    GLint width[6], height[6], nChannels[6];
    GLenum colorModels[6];
    GLubyte *data[6];
    for (GLuint i = 0; i < 6; i++) {
        data[i] = stbi_load(srcPath[i], &width[i], &height[i], &nChannels[i], 0);
        if (!data) {
            printf("[ERROR]In ResourceManager::loadBoxTexture\n");
            printf("\tError loading file\n");
            return NULL;
        }
        if (nChannels[i] == 3)
            colorModels[i] = GL_RGB;
        else
            colorModels[i] = GL_RGBA;
    }

    textures[n] = new Texture(GL_TEXTURE_CUBE_MAP, data, colorModels,
        width, height);
    for (GLuint i = 0; i < 6; i++)
        stbi_image_free(data[i]);
    return textures[n];
}

Renderer *ResourceManager::loadRenderer(const RendererType type, Shader *shader,
    const GLchar *name, Texture *texture) {
    
    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (renderers[n])
        return renderers[n];
    switch (type) {
    case RENDERER_LAND:
        renderers[n] = new LandRenderer(shader, texture);
        break;
    case RENDERER_SKYBOX:
        renderers[n] = new SkyboxRenderer(shader, texture);
        break;
    default:
        printf("[ERROR]In ResourceManager::loadRenderer\n");
        printf("\tWrong renderer type\n");
        return NULL;
    }

    return renderers[n];
}

Model *ResourceManager::loadModel(const GLchar *path, Shader *shader,
    const GLchar *name) {

    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (models[n]) {
        return models[n];
    }

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("[ERROR]In ResourceManager::loadModel\n\t%s\n",
            importer.GetErrorString());
        return NULL;
    }

    std::string str(path);
    models[n] = new Model(shader, scene,
        str.substr(0, str.find_last_of('/')).c_str());
    return models[n];
}

Shader *ResourceManager::getShader(GLchar *name) {
    return shaders[name];
}

Texture *ResourceManager::getTexture(GLchar *name) {
    return textures[name];
}

Renderer *ResourceManager::getRenderer(GLchar *name) {
    return renderers[name];
}

Camera *ResourceManager::getCamera(GLchar *name) {
    return cameras[name];
}

Model *ResourceManager::getModel(GLchar *name) {
    return models[name];
}

ResourceManager::ResourceManager() {}