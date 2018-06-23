#pragma once

#define TITLE "Magic House"

#include "lib/glad.h"
#include "ResourceManager.hpp"
#include "CollisionDetector.hpp"
#include "GUI.hpp"
#include "Frog.hpp"

class Game {
public:
    Game(GLuint width, GLuint height);
    ~Game();

    void init();
    void processInput();
    void render();

    GLFWwindow *getWindow() const;

private:
    GLFWwindow *window;
    GLuint width, height;
	GLuint depthMapFBO;
	GLuint hdrFBO;
	GLuint colorBuffer;
    ResourceManager *resManager;
    CollisionDetector *collisionDetector;
    GUI *gui;
    MyFrog *frog;

    void renderObjects(Camera *camera, Shader *shader = NULL);
	void initFramebuffer();
	void renderQuad();
};
