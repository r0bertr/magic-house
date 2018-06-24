#pragma once

#include "Renderer.hpp"

class WaterRenderer : public Renderer {
public:
	WaterRenderer(Shader *shader, Texture **textures = NULL, Light *light = NULL);
	~WaterRenderer();
	void draw(
		glm::mat4 projection,
		glm::mat4 view,
		glm::vec3 viewPos,
		glm::vec3 pos = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f),
		glm::vec3 rotAxis = glm::vec3(1.f),
		GLfloat rotate = 0.f,
		glm::vec4 color = glm::vec4(1.f),
		glm::vec3 collisionSize = glm::vec3(0.f)
	);

private:
	void initRenderData();
	float k1, k2, k3;
	int n, m;
	GLfloat *data, *pre;
	GLint *indices;
};
