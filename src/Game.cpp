#include "lib/glad.h"
#include "Game.hpp"

#include <cstdio>
#include <GLFW/glfw3.h>

#define __APPLE_RETINA__ false

const GLuint SHADOW_WIDTH = 2048;
const GLuint SHADOW_HEIGHT = 2048;
const GLfloat PI = 3.1415926;
bool hdr = true;
float exposure = 1.0f;

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

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
	gui = new GUI(window);
}

Game::~Game() {
    glfwTerminate();
}

void Game::initFramebuffer() {
	if (__APPLE_RETINA__) {
		width *= 2;
		height *= 2;
	}

	// configure floating point framebuffer
	// ------------------------------------
	glGenFramebuffers(1, &hdrFBO);
	// create floating point color buffer
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::init() {

    // Load Shaders
    Shader *shader = resManager->loadShader("res/shaders/mesh.vs", 
        "res/shaders/mesh.fs", NULL, "mesh");
    Shader *skyboxShader = resManager->loadShader("res/shaders/skybox.vs",
        "res/shaders/skybox.fs", NULL, "skybox");
	Shader *depthShader = resManager->loadShader("res/shaders/depth.vs",
		"res/shaders/depth.fs", NULL, "depth");
	Shader *hdrShader = resManager->loadShader("res/shaders/hdr.vs",
		"res/shaders/hdr.fs", NULL, "hdr");

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
	Texture *nightSkyboxTexture = resManager->loadBoxTexture(nightFaces, "nightSkybox", true);
	Texture *depthMap = resManager->loadDepthTexture(SHADOW_WIDTH, SHADOW_HEIGHT, "depth");

	// HDR framebuffer and shader configuration
	initFramebuffer();
	hdrShader->use();
	hdrShader->uniform1("hdrBuffer", 0);

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
    resManager->loadParticleRenderer(shader,
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
	
	resManager->loadModel("res/models/Frog/Creature.obj",
		shader, light, "frog");
	resManager->getModel("frog")->setTexture(3, depthMap);

    // Load Camera
    resManager->loadCamera(glm::radians(45.f), (float)width / height, .1f, 100.f,
        0.f, 5.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f, "main");

	gui->init();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
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
	if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) {
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(window, NULL);
			gui->enable();
		} else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, mouseCallback);
			gui->disable();
		}
	}
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
		resManager->getRenderer("particle_fire")->setShader(shader);
	resManager->getRenderer("particle_fire")->draw(projection, view, viewPos,
		glm::vec3(0.f, 5.f, -3.f), glm::vec3(.025f),
		glm::vec3(1.f), 0.f, glm::vec4(1.f));

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

	resManager->getModel("frog")->draw(projection, view, viewPos,
		glm::vec3(10.f, 0.5f, -3.f), glm::vec3(3.f),
		glm::vec3(1.f, 0.f, 0.f), 220.f);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void Game::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Game::render() {
    Camera *camera = resManager->getCamera("main");
	Light *light = resManager->getLight("light");
    glm::mat4 projection = camera->getProjection();
    glm::mat4 view = camera->getView();
    glm::vec3 viewPos = camera->getPos();
	static glm::vec3 lastViewPos = camera->getPos();
    camera->jumpCheck();
    if (collisionDetector->judge(viewPos)) {
        camera->setPos(lastViewPos);
    }
	lastViewPos = camera->getPos();
	
	// Shadow rendering
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	renderObjects(camera, resManager->getShader("depth"));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// HDR rendering
	// 1. render scene into floating point framebuffer
	// -----------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderObjects(camera, resManager->getShader("mesh"));
	// float dayAlpha = light->rotate(glm::vec3(7.f, 0.f, -43.f));
	float dayAlpha = 1.f;
	resManager->getRenderer("skybox")->draw(projection, view, viewPos,
		glm::vec3(0.f, -100.f, 0.f), glm::vec3(500.f),
		glm::vec3(1.f), 0.f, glm::vec4(1.f, 1.f, 1.f, dayAlpha));
	resManager->getRenderer("nightSkybox")->draw(projection, view, viewPos,
		glm::vec3(0.f, -100.f, 0.f), glm::vec3(500.f),
		glm::vec3(1.f), 0.f, glm::vec4(1.f, 1.f, 1.f, 1 - dayAlpha));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	// --------------------------------------------------------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	resManager->getShader("hdr")->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	resManager->getShader("hdr")->uniform1("hdr", hdr);
	resManager->getShader("hdr")->uniform1("exposure", exposure);
	renderQuad();

	// std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;

	// Render GUI
	gui->render(((ParticleRenderer *)resManager->getRenderer("particle_fire"))->getAttributes());

}

GLFWwindow *Game::getWindow() const {
    return window;
}