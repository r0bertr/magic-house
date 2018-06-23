#pragma once

#include "lib/glad.h"
#include "lib/imgui_impl_glfw_gl3.h"
#include "Renderers/ParticleRenderer.hpp"

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