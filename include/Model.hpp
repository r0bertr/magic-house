#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "Light.hpp"
#include "Renderers/MeshRenderer.hpp"

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>

class Model {
public:
    Model(Shader *shader, const aiScene *scene, const GLchar *rootDir,
        Light *light);
    ~Model();

    void draw(
        glm::mat4 projection,
        glm::mat4 view,
        glm::vec3 viewPos,
        glm::vec3 pos = glm::vec3(0.f),
        glm::vec3 scale = glm::vec3(1.f),
        glm::vec3 rotAxis = glm::vec3(1.f),
        GLfloat rotate = 0.f,
        glm::vec4 color = glm::vec4(1.f),
        glm::vec3 collisionSize = glm::vec3(0.f)
    );

    void setShader(Shader *shader);
    
    void setTexture(GLuint index, Texture *texture);

private:
    std::vector<MeshRenderer*> *renderers;
    Shader *shader;
    aiString *rootDir;
    Light *light;

    void processNode(const aiNode *node, const aiScene *scene);
    void processMesh(const aiMesh *mesh, const aiScene *scene);
};
