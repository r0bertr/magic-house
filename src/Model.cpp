#include "lib/glad.h"
#include "Model.hpp"

Model::Model(Shader *shader, const aiScene *scene, const GLchar *rootDir) {
    this->shader = shader;
    renderers = new std::vector<MeshRenderer *>;
    this->rootDir = new aiString(rootDir);
    printf("[INFO]Begin model %s\n", rootDir);
    processNode(scene->mRootNode, scene);
    printf("[INFO]End model %s\n", rootDir);
}

Model::~Model() {
    for (GLuint i = 0; i < renderers->size(); i++) {
        delete renderers->at(i);
    }
    delete renderers;
}

void Model::draw(glm::mat4 projection, glm::mat4 view, glm::vec3 pos,
    glm::vec3 scale, glm::vec3 rotAxis, GLfloat rotate) {
    for (GLuint i = 0; i < renderers->size(); i++) {
        renderers->at(i)->draw(projection, view, pos, scale, rotAxis, rotate);
    }
}

void Model::processNode(const aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (GLuint i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(const aiMesh *mesh, const aiScene *scene) {
    renderers->push_back(new MeshRenderer(shader, mesh, scene,
        rootDir->C_Str()));
}