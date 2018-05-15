#pragma once

#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "Renderers/CubeRenderer.hpp"

#include <map>

struct MapCMP {
    bool operator()(const GLchar *keyX, const GLchar *keyY) const {
        return strcmp(keyX, keyY);
    }
};

class ResourceManager {
public:
    static ResourceManager *GetInstance();

    Shader *loadShader(const GLchar *vPath, const GLchar *fPath,
        const GLchar *aPath, const GLchar *name);
    Texture *load2DTexture(const GLchar *srcPath, const GLchar *name);
    Renderer *loadRenderer(const RendererType type, Shader *shader,
        const GLchar *modelPath, const GLchar *name, Texture *texture = NULL);
    Camera *loadCamera(GLfloat pFov, GLfloat pAspect, GLfloat pNear, GLfloat pFar,
        GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat frontX, GLfloat frontY, GLfloat frontZ,
        GLfloat upX, GLfloat upY, GLfloat upZ, const GLchar *name);
    Model *loadModel(const GLchar *path, Shader *shader, const GLchar *name);

    Shader *getShader(GLchar *name);
    Texture *getTexture(GLchar *name);
    Renderer *getRenderer(GLchar *name);
    Camera *getCamera(GLchar *name);
    Model *getModel(GLchar *name);
private:
    std::map<GLchar*, Shader*, MapCMP> shaders;
    std::map<GLchar*, Texture*, MapCMP> textures;
    std::map<GLchar*, Renderer*, MapCMP> renderers;
    std::map<GLchar*, Camera*, MapCMP> cameras;
    std::map<GLchar*, Model*, MapCMP> models;

    ResourceManager();
};
