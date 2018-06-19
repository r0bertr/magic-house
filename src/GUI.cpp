#include "GUI.hpp"
#include "lib/imgui_impl_glfw_gl3.h"

GUI::GUI(GLFWwindow *window)
    : window(window), enabled(false) {}

GUI::~GUI() {
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

void GUI::init() {

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, false);

}

void GUI::render(ParticleAttributes *parAttrs) {

    if (!enabled)
        return;

    ImGui_ImplGlfwGL3_NewFrame();

    renderParticleConsole(parAttrs);

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

}

void GUI::enable() {
    enabled = true;
}

void GUI::disable() {
    enabled = false;
}

void GUI::renderParticleConsole(ParticleAttributes *parAttrs) {

    ImGui::Begin("Particle System Console");
    ImGui::SetWindowSize(ImVec2(600, 400));
    ImGui::SliderInt("num_particles_mean",
        (int *)&parAttrs->numParticles.mean, 1, 100);
    ImGui::SliderInt("num_particles_variance",
        (int *)&parAttrs->numParticles.variance, 1, 100);
    ImGui::SliderFloat3("init_position_mean",
        glm::value_ptr(parAttrs->initPosition.mean), -50.f, 50.f);
    ImGui::SliderFloat3("init_position_variance",
        glm::value_ptr(parAttrs->initPosition.variance), -50.f, 50.f);
    ImGui::SliderFloat3("init_direction_mean",
        glm::value_ptr(parAttrs->initDirection.mean), -50.f, 50.f);
    ImGui::SliderFloat3("init_direction_variance",
        glm::value_ptr(parAttrs->initDirection.variance), -50.f, 50.f);
    ImGui::SliderFloat("init_speed_mean",
        &parAttrs->initSpeed.mean, -1.f, 1.f);
    ImGui::SliderFloat("init_speed_variance",
        &parAttrs->initSpeed.variance, -1.f, 1.f);
    ImGui::SliderFloat3("init_size_mean",
        glm::value_ptr(parAttrs->initSize.mean), -50.f, 50.f);
    ImGui::SliderFloat3("init_size_variance",
        glm::value_ptr(parAttrs->initSize.variance), -50.f, 50.f);
    ImGui::SliderFloat4("init_color_mean",
        glm::value_ptr(parAttrs->initColor.mean), 0.f, 1.f);
    ImGui::SliderFloat4("init_color_variance",
        glm::value_ptr(parAttrs->initColor.variance), 0.f, 1.f);
    ImGui::SliderInt("init_life",
        (int *)&parAttrs->initLife, 1, 100);
    ImGui::Checkbox("gravity",
        (bool *)&parAttrs->gravity);
    ImGui::SliderFloat3("gravity_dir",
        glm::value_ptr(parAttrs->gravityDir), -50.f, 50.f);
    ImGui::SliderFloat("gravity_mag",
        &parAttrs->gravityMag, 0.f, 100.f);
    ImGui::End();

}