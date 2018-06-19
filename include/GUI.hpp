#pragma once

#include "Renderers/ParticleRenderer.hpp"
#include <GLFW/glfw3.h>

class GUI {
public:
    GUI(GLFWwindow *window);
    ~GUI();

    void init();
    void render(ParticleAttributes *parAttrs);
    void enable();
    void disable();

private:
    GLFWwindow *window;
    bool enabled;

    void renderParticleConsole(ParticleAttributes *parAttrs);
};