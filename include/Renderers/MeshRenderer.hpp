#pragma once

#include "Renderer.hpp"

#include <assimp/scene.h>
#include <vector>

class MeshRenderer : public Renderer {
public:
    MeshRenderer(Shader *shader, const aiMesh *mesh, const aiScene *scene,
        const GLchar *rootDir);
    ~MeshRenderer();

    void draw(glm::mat4 projection, glm::mat4 view,
        glm::vec3 pos = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f), GLfloat rotate = 0.f, glm::vec3 viewPos = glm::vec3(0.f));

private:
    std::vector<GLfloat> *vertices;
    std::vector<GLuint> *indices;
    std::vector<Texture *> *textures;
    GLchar rootDir[64];

    void initRenderData();
    void processTextures(aiMaterial *material, aiTextureType type);
};
