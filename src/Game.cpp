#include "lib/glad.h"
#include "Game.hpp"

#include <cstdio>
#include <GLFW/glfw3.h>

const GLuint SHADOW_WIDTH = 2048;
const GLuint SHADOW_HEIGHT = 2048;

Game::Game(GLuint width, GLuint height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
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
    collisionDetector = CollisionDetector::getInstance();
}

Game::~Game() {
    glfwTerminate();
}

void Game::init() {
    // Load Shaders
    Shader *shader = resManager->loadShader("res/shaders/mesh.vs", 
        "res/shaders/mesh.fs", NULL, "mesh");
    Shader *skyboxShader = resManager->loadShader("res/shaders/skybox.vs",
        "res/shaders/skybox.fs", NULL, "skybox");
	Shader *particleShader = resManager->loadShader("res/shaders/particle.vs",
		"res/shaders/particle.fs", NULL, "particle");
	Shader *depthShader = resManager->loadShader("res/shaders/depth.vs",
		"res/shaders/depth.fs", NULL, "depth");

    // Load Textures
    Texture *grass = resManager->load2DTexture("res/images/grass.jpg",
        "grass", true);
	// Day Skybox
    const GLchar *faces[6] = {
        "res/images/skybox/right.jpg",
        "res/images/skybox/left.jpg",
        "res/images/skybox/top.jpg",
        "res/images/skybox/bottom.jpg",
        "res/images/skybox/back.jpg",
        "res/images/skybox/front.jpg"
    };
	// Night Skybox
	const GLchar *nightFaces[6] = {
		"res/images/nightSky/Spacebox2/right.png",
		"res/images/nightSky/Spacebox2/left.png",
		"res/images/nightSky/Spacebox2/top.png",
		"res/images/nightSky/Spacebox2/bottom.png",
		"res/images/nightSky/Spacebox2/back.png",
		"res/images/nightSky/Spacebox2/front.png"
	};

    Texture *skyboxTexture = resManager->loadBoxTexture(faces, "skybox", true);
    Texture *smokeTexture = resManager->load2DTexture("res/images/smoke.png",
        "smoke", true);
	Texture *nightSkyboxTexture = resManager->loadBoxTexture(nightFaces, "nightSkybox", true);
	Texture *depthMap = resManager->loadDepthTexture(SHADOW_WIDTH, SHADOW_HEIGHT, "depth");

	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
        depthMap->getID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Load Lights
    Light *light = resManager->loadLight("light", LIGHT_DIRECT,
        glm::vec3(0.f, 0.f, 500.f), glm::vec3(1.f), 
        glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, .1f, 500.f),
		.4f, 1.f, .5f, glm::vec3(0.f, 0.f, 1.f));
    // Load Renderers
    Texture *buffer[16] = { NULL };
	buffer[0] = grass; buffer[3] = depthMap;
    resManager->loadRenderer(RENDERER_LAND, shader, "land", buffer, light);
	buffer[0] = skyboxTexture; buffer[3] = NULL;
    resManager->loadRenderer(RENDERER_SKYBOX, skyboxShader,
        "skybox", buffer);
    resManager->loadParticleRenderer(particleShader, smokeTexture,
        "res/configs/particle_fire.json", "particle_fire");
	buffer[0] = nightSkyboxTexture; buffer[3] = NULL;
	resManager->loadRenderer(RENDERER_SKYBOX, skyboxShader,
		"nightSkybox", buffer);
    
    // Load Models
    resManager->loadModel("res/models/Farmhouse/farmhouse_obj.obj",
        shader, light, "farmhouse");
    resManager->getModel("farmhouse")->setTexture(3, depthMap);

	resManager->loadModel("res/models/tree/tree1.obj",
		shader, light, "tree");
    resManager->getModel("tree")->setTexture(3, depthMap);
	
	resManager->loadModel("res/models/fence/Fence_White.obj",
		shader, light, "woodenfence");
    resManager->getModel("woodenfence")->setTexture(3, depthMap);

    // Load Camera
    resManager->loadCamera(glm::radians(45.f), (float)width / height, .1f, 100.f,
        0.f, 5.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f, "main");
}

void Game::processInput() {
    static float lastTime = 0.f;
    Camera *camera = resManager->getCamera("main");
    float curTime = (float)glfwGetTime();
    float moveSpeed = 20.f * (curTime - lastTime);
    lastTime = curTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward2D(moveSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBack2D(moveSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveLeft2D(moveSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight2D(moveSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->jump();
}

void Game::renderObjects(Camera *camera, Shader *shader) {
	
	glm::mat4 projection = camera->getProjection();
	glm::mat4 view = camera->getView();
	glm::vec3 viewPos = camera->getPos();

    if (shader)
        resManager->getRenderer("land")->setShader(shader);
    resManager->getRenderer("land")->draw(projection, view, viewPos,
		glm::vec3(0.f), glm::vec3(1000.f),
		glm::vec3(1.f, 0.f, 0.f), 90.f);

	if (shader)
		resManager->getModel("farmhouse")->setShader(shader);
	resManager->getModel("farmhouse")->draw(projection, view, viewPos,
        glm::vec3(7.f, 0.f, -50.f), glm::vec3(.8f),
        glm::vec3(0.f, 1.f, 0.f), 180.f, glm::vec4(0.f),
        glm::vec3(18.f, 50.f, 30.f));

	if (shader)
		resManager->getModel("tree")->setShader(shader);
	resManager->getModel("tree")->draw(projection, view, viewPos,
		glm::vec3(30.f, -2.f, -35.f), glm::vec3(1.f),
        glm::vec3(1.f), 0.f, glm::vec4(0.f),
        glm::vec3(2.f, 50.f, 2.f));

	if (shader)
		resManager->getModel("woodenfence")->setShader(shader);
    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(0, 0.f, -20.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 90.f, glm::vec4(1.f),
        glm::vec3(24.f, 10.f, 3.f));

    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(30.f, 0.f, -20.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 90.f, glm::vec4(1.f),
        glm::vec3(24.f, 20.f, 3.f));

    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(0.f, 0.f, -68.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 90.f, glm::vec4(1.f),
        glm::vec3(24.f, 20.f, 3.f));

    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(30.f, 0.f, -68.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 90.f, glm::vec4(1.f),
        glm::vec3(24.f, 20.f, 3.f));

    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(-13.f, 0.f, -33.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 0.f, glm::vec4(1.f),
        glm::vec3(3.f, 20.f, 24.f));

    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(-13.f, 0.f, -57.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 0.f, glm::vec4(1.f),
        glm::vec3(3.f, 20.f, 24.f));

    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(41.f, 0.f, -33.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 0.f, glm::vec4(1.f),
        glm::vec3(3.f, 20.f, 24.f));

    resManager->getModel("woodenfence")->draw(projection, view, viewPos,
		glm::vec3(41.f, 0.f, -57.f), glm::vec3(0.25f, 0.15f, 0.25f),
		glm::vec3(0.f, 1.f, 0.f), 0.f, glm::vec4(1.f),
        glm::vec3(3.f, 20.f, 24.f));

}

void Game::render() {
    Camera *camera = resManager->getCamera("main");
	Light *light = resManager->getLight("light");
    glm::mat4 projection = camera->getProjection();
    glm::mat4 view = camera->getView();
    glm::vec3 viewPos = camera->getPos();
    camera->jumpCheck();

    if (collisionDetector->judge(viewPos)) {
        camera->undoMove();
    }

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	renderObjects(camera, resManager->getShader("depth"));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderObjects(camera, resManager->getShader("mesh"));

    float dayAlpha = light->rotate(glm::vec3(7.f, 0.f, -43.f));

    resManager->getRenderer("skybox")->draw(projection, view, viewPos,
        glm::vec3(0.f, -100.f, 0.f), glm::vec3(500.f),
        glm::vec3(1.f), 0.f, glm::vec4(1.f, 1.f, 1.f, dayAlpha));

	resManager->getRenderer("nightSkybox")->draw(projection, view, viewPos,
		glm::vec3(0.f, -100.f, 0.f), glm::vec3(500.f),
        glm::vec3(1.f), 0.f, glm::vec4(1.f, 1.f, 1.f, 1 - dayAlpha));

}

GLFWwindow *Game::getWindow() const {
    return window;
}