#pragma once

#include "Renderers/ParticleRenderer.hpp"
#include <GLFW/glfw3.h>

class GUI {
public:
    GUI(GLFWwindow *window);
    ~GUI();

    void init();
    void render(ParticleAttributes *parAttrs);

private:
    GLFWwindow *window;

    void renderParticleConsole(ParticleAttributes *parAttrs);
};