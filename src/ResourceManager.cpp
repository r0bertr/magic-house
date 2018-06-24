#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "ResourceManager.hpp"
#include "Renderers/ParticleRenderer.hpp"
#include "Renderers/LandRenderer.hpp"
#include "Renderers/SkyboxRenderer.hpp"
#include "Renderers/WaterRenderer.hpp"

#include <cstdio>
#include <cstring>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define BUFFER_LEN 32

char *const2var(const char *c) {
	int n = strlen(c);
	char *var = new char[n + 1];
	strcpy(var, c);
	return var;
}

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
        delete []n;
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
        delete []n;
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

Texture *ResourceManager::load2DTexture(const GLchar *srcPath, const GLchar *name, bool gammaCorrection) {
    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (textures[n]) {
        return textures[n];
        delete []n;
    }

    GLint width, height, nChannels;
    GLubyte *data = stbi_load(srcPath, &width, &height, &nChannels, 0);
	GLenum internalFormat;
	GLenum dataFormat;
    if (!data) {
        printf("[ERROR]In ResourceManager::load2DTexture\n");
        printf("\tError loading file\n");
        return NULL;
    }

	if (nChannels == 3)
	{
		internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
		dataFormat = GL_RGB;
	}
	else {
		internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
		dataFormat = GL_RGBA;
	}

    textures[n] = new Texture(GL_TEXTURE_2D, data, internalFormat, dataFormat, width, height);
    stbi_image_free(data);
    return textures[n];

}

Texture *ResourceManager::loadDepthTexture(const int SHADOW_WIDTH, const int SHADOW_HEIGHT, const GLchar *name) {
	GLchar *n = new GLchar[BUFFER_LEN];
	strcpy(n, name);
	if (textures[n]) {
		return textures[n];
		delete[]n;
	}
	textures[n] = new Texture(GL_TEXTURE_2D, SHADOW_WIDTH, SHADOW_HEIGHT);
	return textures[n];

}
Renderer *ResourceManager::loadParticleRenderer(Shader *shader,
    const GLchar *config, const GLchar *name,
    Light *light) {

    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (renderers[n]) {
        return renderers[n];
        delete []n;
    }

    Renderer *renderer = new ParticleRenderer(shader, config, light);
    renderers[n] = renderer;
    return renderer;

}

Texture *ResourceManager::loadBoxTexture(const GLchar *srcPath[6],
    const GLchar *name, bool gammaCorrection) {
    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (textures[n]) {
        return textures[n];
        delete []n;
    }

    GLint width[6], height[6], nChannels[6];
    GLenum internalFormats[6];
	GLenum dataFormats[6];
    GLubyte *data[6];
    for (GLuint i = 0; i < 6; i++) {
        data[i] = stbi_load(srcPath[i], &width[i], &height[i], &nChannels[i], 0);
        if (!data) {
            printf("[ERROR]In ResourceManager::loadBoxTexture\n");
            printf("\tError loading file\n");
            return NULL;
        }
		if (nChannels[i] == 3) {
			internalFormats[i] = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormats[i] = GL_RGB;
		}
		else {
			internalFormats[i] = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormats[i] = GL_RGBA;
		}
    }

    textures[n] = new Texture(GL_TEXTURE_CUBE_MAP, data, internalFormats, dataFormats,
        width, height);
    for (GLuint i = 0; i < 6; i++)
        stbi_image_free(data[i]);
    return textures[n];
}

Renderer *ResourceManager::loadRenderer(const RendererType type, Shader *shader,
    const GLchar *name, Texture **textures, Light *light) {
    
    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (renderers[n]) {
        return renderers[n];
        delete []n;
    }
    switch (type) {
    case RENDERER_LAND:
        renderers[n] = new LandRenderer(shader, textures, light);
        break;
	case RENDERER_WATER:
		renderers[n] = new WaterRenderer(shader, textures, light);
		break;
    case RENDERER_SKYBOX:
        renderers[n] = new SkyboxRenderer(shader, textures, light);
        break;
    default:
        printf("[ERROR]In ResourceManager::loadRenderer\n");
        printf("\tWrong renderer type\n");
        return NULL;
    }

    return renderers[n];
}

Model *ResourceManager::loadModel(const GLchar *path, Shader *shader,
    Light *light, const GLchar *name) {

    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (models[n]) {
        return models[n];
        delete []n;
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
        str.substr(0, str.find_last_of('/')).c_str(), light);
    return models[n];
}

Light *ResourceManager::loadLight(const GLchar *name, 
    LightType type, glm::vec3 position, glm::vec3 color, glm::mat4 projection,
    GLfloat ambient, GLfloat diffuse, GLfloat specular,
    glm::vec3 direction) {

    GLchar *n = new GLchar[BUFFER_LEN];
    strcpy(n, name);
    if (lights[n]) {
        Light *light = lights[n];
        delete []n;
        return light;
    }

    lights[n] = new Light(type, position, color, direction, projection,
        ambient, diffuse, specular);

    return lights[n];
}

Shader *ResourceManager::getShader(const GLchar *name) {
	GLchar *nameStr = const2var(name);
	Shader *shader = shaders[nameStr];
	delete nameStr;
	return shader;
}

Texture *ResourceManager::getTexture(const GLchar *name) {
	GLchar *nameStr = const2var(name);
	Texture *texture = textures[nameStr];
	delete []nameStr;
	return texture;
}

Renderer *ResourceManager::getRenderer(const GLchar *name) {
	GLchar *nameStr = const2var(name);
	Renderer *renderer = renderers[nameStr];
	delete []nameStr;
	return renderer;
}

Camera *ResourceManager::getCamera(const GLchar *name) {
	GLchar *nameStr = const2var(name);
	Camera *camera = cameras[nameStr];
	delete []nameStr;
    return camera;
}

Light *ResourceManager::getLight(const GLchar *name) {
	GLchar *nameStr = const2var(name);
	Light *light = lights[nameStr];
	delete []nameStr;
	return light;
}

Model *ResourceManager::getModel(const GLchar *name) {
	GLchar *nameStr = const2var(name);
	Model *model = models[nameStr];
	delete []nameStr;
	return model;
}

ResourceManager::ResourceManager() {}