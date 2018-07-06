#include "Model.hpp"

Model::Model(Shader *shader, const aiScene *scene, const GLchar *rootDir,
    Light *light) {
    this->shader = shader;
    this->light = light;
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

void Model::draw(glm::mat4 projection, glm::mat4 view,
    glm::vec3 viewPos, glm::vec3 pos, glm::vec3 scale,
    glm::vec3 rotAxis, GLfloat rotate, glm::vec4 color,
    glm::vec3 collisionSize) {
    for (GLuint i = 0; i < renderers->size(); i++) {
        renderers->at(i)->draw(projection, view, viewPos, pos, scale, 
            rotAxis, rotate, color, collisionSize);
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
        rootDir->C_Str(), light));
}

void Model::setTexture(GLuint index, Texture *texture) {

    for (GLuint i = 0; i < renderers->size(); i++) {
        renderers->at(i)->setTexture(index, texture);
    }

}

void Model::setShader(Shader *shader) {

    this->shader = shader;
    for (GLuint i = 0; i < renderers->size(); i++) {
        renderers->at(i)->setShader(shader);
    }

}