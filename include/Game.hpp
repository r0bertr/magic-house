#pragma once

#include "ResourceManager.hpp"

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
    ResourceManager *resManager;
};
