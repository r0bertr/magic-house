#include "lib/glad.h"
#include "Game.hpp"

#include <GLFW/glfw3.h>

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = 0.f, lastY = 0.f;
    static float yaw = -90.f, pitch = 0.f;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = .01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    ResourceManager::GetInstance()->getCamera("main")->rotate(pitch, yaw);
}

int main() {
    Game game(WIDTH, HEIGHT);
    game.init();

    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(game.getWindow(), mouseCallback);

    // Main Loop
    while (!glfwWindowShouldClose(game.getWindow())) {
        glfwPollEvents();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game.processInput();
        game.render();

        glfwSwapBuffers(game.getWindow());
    }

    return 0;
}