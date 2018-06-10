#pragma once

#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "Light.hpp"
#include "Renderers/LandRenderer.hpp"
#include "Renderers/SkyboxRenderer.hpp"
#include "Renderers/ParticleRenderer.hpp"

#include <map>

struct MapCMP {
    bool operator()(const GLchar *keyX, const GLchar *keyY) const {
        std::string x(keyX), y(keyY);
        return x < y;
    }
};

class ResourceManager {
public:
    static ResourceManager *GetInstance();

    Shader *loadShader(const GLchar *vPath, const GLchar *fPath,
        const GLchar *aPath, const GLchar *name);

    Texture *load2DTexture(const GLchar *srcPath, const GLchar *name);
    Texture *loadBoxTexture(const GLchar *srcPath[6], const GLchar *name);
	Texture *ResourceManager::loadDepthTexture(const int SHADOW_WIDTH, const int SHADOW_HEIGHT, const GLchar *name);

    Renderer *loadRenderer(const RendererType type, Shader *shader,
        const GLchar *name, Texture **textures = NULL, Light *light = NULL);
    Renderer *loadParticleRenderer(Shader *shader, Texture *texture,
        const GLchar *config, const GLchar *name, Light *light = NULL);

    Camera *loadCamera(GLfloat pFov, GLfloat pAspect, GLfloat pNear, GLfloat pFar,
        GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat frontX, GLfloat frontY, GLfloat frontZ,
        GLfloat upX, GLfloat upY, GLfloat upZ, const GLchar *name);

    Model *loadModel(const GLchar *path, Shader *shader, Light *light, const GLchar *name);

    Light *loadLight(const GLchar *name, 
        LightType type, glm::vec3 position, glm::vec3 color,
        GLfloat ambient, GLfloat diffuse, GLfloat specular,
        glm::vec3 direction = glm::vec3(0.f));

    Shader *getShader(GLchar *name);
    Texture *getTexture(GLchar *name);
    Renderer *getRenderer(GLchar *name);
    Camera *getCamera(GLchar *name);
    Model *getModel(GLchar *name);
	Light *getLight(GLchar *name);
private:
    std::map<GLchar*, Shader*, MapCMP> shaders;
    std::map<GLchar*, Texture*, MapCMP> textures;
    std::map<GLchar*, Renderer*, MapCMP> renderers;
    std::map<GLchar*, Camera*, MapCMP> cameras;
    std::map<GLchar*, Model*, MapCMP> models;
    std::map<GLchar*, Light*, MapCMP> lights;

    ResourceManager();
};
