#include "lib/glad.h"
#include "ResourceManager.hpp"
#include "Renderers/MeshRenderer.hpp"
#include "Model.hpp"

#include <assimp/scene.h>
#include <cstring>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

MeshRenderer::MeshRenderer(Shader *shader, const aiMesh *mesh,
    const aiScene *scene, const GLchar *rootDir, Light *light) :
    Renderer(shader, NULL, light) {
    vertices = new std::vector<GLfloat>;
    indices = new std::vector<GLuint>;
    strcpy(this->rootDir, rootDir);

    for (GLuint i = 0; i < 16; i++) {
        textures[i] = NULL;
    }

    for (GLuint i = 0; i < mesh->mNumVertices; i++) {
        vertices->push_back(mesh->mVertices[i].x);
        vertices->push_back(mesh->mVertices[i].y);
        vertices->push_back(mesh->mVertices[i].z);
        if (mesh->mTextureCoords[0]) {
            vertices->push_back(mesh->mTextureCoords[0][i].x);
            vertices->push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices->push_back(0.f);
            vertices->push_back(0.f);
        }
        vertices->push_back(mesh->mNormals[i].x);
        vertices->push_back(mesh->mNormals[i].y);
        vertices->push_back(mesh->mNormals[i].z);
    }

    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
        aiFace *face = &mesh->mFaces[i];
        for (GLuint j = 0; j < face->mNumIndices; j++) {
            indices->push_back(face->mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        if (material->GetTextureCount(aiTextureType_AMBIENT) > 0) {
            processTextures(material, aiTextureType_AMBIENT);
        }
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            processTextures(material, aiTextureType_DIFFUSE);
        }
        if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            processTextures(material, aiTextureType_SPECULAR);
        }
    }

    initRenderData();
}

void MeshRenderer::processTextures(aiMaterial *material, aiTextureType type) {
    GLint slot = 0;
    if (type == aiTextureType_DIFFUSE) {
        slot = 0;
    } else if (type == aiTextureType_SPECULAR) {
        slot = 1;
    } else if (type == aiTextureType_SHININESS) {
        slot = 2;
    }

    aiString texturePath(rootDir), temp;
    material->GetTexture(type, 0, &temp);
    texturePath.Append("/");
    texturePath.Append(temp.C_Str());
    printf("[INFO]In MeshRenderer::processTextures\n");
    printf("\tProcessing texture %s\n", texturePath.C_Str());
    textures[slot] = ResourceManager::GetInstance()->load2DTexture(
        texturePath.C_Str(), temp.C_Str());
}

MeshRenderer::~MeshRenderer() {
    delete vertices;
    delete indices;
}

void MeshRenderer::initRenderData() {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(GLfloat),
        vertices->data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
        (void *)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint),
        indices->data(), GL_STATIC_DRAW);
    numVertices = indices->size();
}