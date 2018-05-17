#include "lib/glad.h"
#include "Game.hpp"

#include <cstdio>
#include <GLFW/glfw3.h>

Game::Game(GLuint width, GLuint height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, TITLE, NULL, NULL);
    if (window == NULL) {
        printf("[ERROR]glfwCreateWindow failed\n");
        return;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    this->width = width;
    this->height = height;
    resManager = ResourceManager::GetInstance();
}

Game::~Game() {
    glfwTerminate();
}

void Game::init() {
    // Load Shaders
    Shader *meshShader = resManager->loadShader("res/shaders/mesh.vs", 
        "res/shaders/mesh.fs", NULL, "mesh");
    Shader *landShader = resManager->loadShader("res/shaders/land.vs",
        "res/shaders/land.fs", NULL, "land");

    // Load Textures
    Texture *grass = resManager->load2DTexture("res/images/grass.jpg",
        "grass");

    // Load Renderers
    resManager->loadRenderer(RENDERER_LAND, landShader, NULL, "land", grass);
    
    // Load Models
    resManager->loadModel("res/models/Farmhouse/farmhouse_obj.obj",
        meshShader, "farmhouse");

    // Load Camera
    resManager->loadCamera(glm::radians(45.f), (float)width / height, .1f, 100.f,
        0.f, 5.f, 5.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f, "main");
}

void Game::processInput() {
    Camera *camera = resManager->getCamera("main");
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(.1f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBack(.1f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveLeft(.1f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight(.1f);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        camera->rotateLeft(.05f);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        camera->rotateRight(.05f);
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera->rotateUp(.05f);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        camera->rotateDown(.05f);
}

void Game::render() {
    Camera *camera = resManager->getCamera("main");
    glm::mat4 projection = camera->getProjection();
    glm::mat4 view = camera->getView();

    resManager->getRenderer("land")->draw(projection, view,
        glm::vec3(0.f), glm::vec3(100.f),
        glm::vec3(1.f, 0.f, 0.f), 90.f);
    
    resManager->getModel("farmhouse")->draw(projection, view,
        glm::vec3(20.f, 0.f, 0.f), glm::vec3(1.f),
        glm::vec3(0.f, 1.f, 0.f), 90.f);
}

GLFWwindow *Game::getWindow() const {
    return window;
}