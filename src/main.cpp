#include "lib/glad.h"
#include "Game.hpp"

#include <GLFW/glfw3.h>

const GLfloat PI = 3.1415926;

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = 0.f, lastY = 0.f;
    static float yaw = -90.f, pitch = 0.f;
    static float lastTime = 0.f;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float curTime = (float)glfwGetTime();
    float sensitivity = .2f * (curTime - lastTime);
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    lastTime = curTime;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > PI / 2 - 0.01f)
        pitch = PI / 2 - 0.01f;
    if (pitch < -PI / 2 + 0.01f)
        pitch = -PI / 2 + 0.01f;

    ResourceManager::GetInstance()->getCamera("main")->rotate(pitch, yaw);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main() {
    Game game(WIDTH, HEIGHT);
    game.init();

    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(game.getWindow(), mouseCallback);
    glfwSetFramebufferSizeCallback(game.getWindow(), framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

    // Main Loop
    while (!glfwWindowShouldClose(game.getWindow())) {
        glfwPollEvents();

        game.processInput();
        game.render();

		// glEnable(GL_FRAMEBUFFER_SRGB);
        glfwSwapBuffers(game.getWindow());
    }

    return 0;
}