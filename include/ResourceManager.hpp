#pragma once

#include <map>

#include "Shader.hpp"
#include "Texture.hpp"
#include "SpriteRenderer.hpp"
#include "Sprites/CubeRenderer.hpp"
#include "Camera.hpp"

class ResourceManager {
public:
    static ResourceManager *GetInstance();

    Shader *loadShader(const GLchar *vPath, const GLchar *fPath,
        const GLchar *aPath, GLchar *name);
    Texture *load2DTexture(const GLchar *srcPath, GLchar *name);
    SpriteRenderer *loadRenderer(const SpriteType type,
        Shader *shader, GLchar *name);
    Camera *loadCamera(GLfloat pFov, GLfloat pAspect, GLfloat pNear, GLfloat pFar,
        GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat frontX, GLfloat frontY, GLfloat frontZ,
        GLfloat upX, GLfloat upY, GLfloat upZ, GLchar *name);

    Shader *getShader(GLchar *name);
    Texture *getTexture(GLchar *name);
    SpriteRenderer *getRenderer(GLchar *name);
    Camera *getCamera(GLchar *name);
private:
    std::map<GLchar*, Shader*> shaders;
    std::map<GLchar*, Texture*> textures;
    std::map<GLchar*, SpriteRenderer*> renderers;
    std::map<GLchar*, Camera*> cameras;

    ResourceManager();
};