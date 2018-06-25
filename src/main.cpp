#include "lib/glad.h"
#include "Game.hpp"

#include <GLFW/glfw3.h>

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

int main() {
    Game game(WIDTH, HEIGHT);
    game.init();

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