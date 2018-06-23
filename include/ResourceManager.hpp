#pragma once

#include "lib/glad.h"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Camera.hpp"

#include <string>
#include <map>

char *const2var(const char *c);

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

    Texture *load2DTexture(const GLchar *srcPath, const GLchar *name, bool gammaCorrection);
    Texture *loadBoxTexture(const GLchar *srcPath[6], const GLchar *name, bool gammaCorrection);
	Texture *loadDepthTexture(const int SHADOW_WIDTH, const int SHADOW_HEIGHT, const GLchar *name);

    Renderer *loadRenderer(const RendererType type, Shader *shader,
        const GLchar *name, Texture **textures = NULL, Light *light = NULL); 
    Renderer *loadParticleRenderer(Shader *shader,
        const GLchar *config, const GLchar *name, Light *light = NULL);

    Camera *loadCamera(GLfloat pFov, GLfloat pAspect, GLfloat pNear, GLfloat pFar,
        GLfloat posX, GLfloat posY, GLfloat posZ,
        GLfloat frontX, GLfloat frontY, GLfloat frontZ,
        GLfloat upX, GLfloat upY, GLfloat upZ, const GLchar *name);

    Model *loadModel(const GLchar *path, Shader *shader, Light *light, const GLchar *name);

    Light *loadLight(const GLchar *name, 
        LightType type, glm::vec3 position, glm::vec3 color, glm::mat4 projection,
        GLfloat ambient, GLfloat diffuse, GLfloat specular,
        glm::vec3 direction = glm::vec3(0.f));

    Shader *getShader(const GLchar *name);
    Texture *getTexture(const GLchar *name);
    Renderer *getRenderer(const GLchar *name);
    Camera *getCamera(const GLchar *name);
    Model *getModel(const GLchar *name);
	Light *getLight(const GLchar *name);
private:
    std::map<GLchar*, Shader*, MapCMP> shaders;
    std::map<GLchar*, Texture*, MapCMP> textures;
    std::map<GLchar*, Renderer*, MapCMP> renderers;
    std::map<GLchar*, Camera*, MapCMP> cameras;
    std::map<GLchar*, Model*, MapCMP> models;
    std::map<GLchar*, Light*, MapCMP> lights;

    ResourceManager();
};
