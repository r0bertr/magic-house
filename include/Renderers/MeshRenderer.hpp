#pragma once

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Light.hpp"

#include <vector>
#include <assimp/scene.h>

class MeshRenderer : public Renderer {
public:
    MeshRenderer(Shader *shader, const aiMesh *mesh, const aiScene *scene,
        const GLchar *rootDir, Light *light);
    ~MeshRenderer();

private:
    std::vector<GLfloat> *vertices;
    std::vector<GLuint> *indices;
    GLchar rootDir[64];

    void initRenderData();
    void processTextures(aiMaterial *material, aiTextureType type);
};
