#pragma once

#include "ResourceManager.hpp"
#include "CollisionDetector.hpp"

#define TITLE "Magic House"

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
    ResourceManager *resManager;
    CollisionDetector *collisionDetector;

    void renderObjects(Camera *camera, Shader *shader = NULL);
};
