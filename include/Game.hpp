#pragma once

#include "ResourceManager.hpp"

#define TITLE "Magic House"

class Game {
public:
    Game(GLuint width, GLuint height, GLuint SHADOW_WIDTH, GLuint SHADOW_HEIGHT);
    ~Game();

    void init();
    void processInput();
    void render();

    GLFWwindow *getWindow() const;

private:
    GLFWwindow *window;
    GLuint width, height;
	GLuint SHADOW_WIDTH, SHADOW_HEIGHT;
	GLuint depthMapFBO;
    ResourceManager *resManager;
};
