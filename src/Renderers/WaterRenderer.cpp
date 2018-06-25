#include "lib/glad.h"
#include "Renderers/WaterRenderer.hpp"

#include <cstdio>
#include <cstring>

WaterRenderer::WaterRenderer(Shader *shader, Texture **textures,
	Light *light) : Renderer(shader, textures, light) {
	initRenderData();
}

WaterRenderer::~WaterRenderer() {
	if (data != NULL) delete[] data;
}

void WaterRenderer::initRenderData() {
	n = 20; m = 400;
	float d = 1.f, t = 0.5f, c = 0.01f, mu = 0.01f;
	int count = n*m;
	float f1 = c * c * t * t / (d * d);
	float f2 = 1.0f / (mu * t + 2);
	k1 = (4.0f - 8.0f * f1) * f2;
	k2 = (mu * t - 2) * f2;
	k3 = 2.0f * f1 * f2;

	numVertices = 6 * (m - 1) * (n - 1);
	data = new GLfloat[count * 8];
	pre = new GLfloat[count * 8];
	indices = new GLint[numVertices];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			data[(i*n + j) * 8] = i*d;
			data[(i*n + j) * 8 + 1] = j*d;
			if (i == 0 || j == 0 || i == m - 1 || j == n - 1) {
				data[(i*n + j) * 8 + 2] = 0.f;
			}
			else {
				int r = rand();
				if (r % 2 == 0) {
					data[(i*n + j) * 8 + 2] = -1.f;
				}
				else {
					data[(i*n + j) * 8 + 2] = 0.f;
				}
			}
			data[(i*n + j) * 8 + 3] = 100 * (i % 2);
			data[(i*n + j) * 8 + 4] = 100 * (1 - j % 2);
			data[(i*n + j) * 8 + 5] = 0.f;
			data[(i*n + j) * 8 + 6] = 0.f;
			data[(i*n + j) * 8 + 7] = -1.f;
		}
	}
	int a = 0;
	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			indices[a * 6] = i*n + j;
			indices[a * 6 + 1] = (i + 1)*n + j;
			indices[a * 6 + 2] = (i + 1)*n + j + 1;
			indices[a * 6 + 3] = i*n + j;
			indices[a * 6 + 4] = i*n + j + 1;
			indices[a * 6 + 5] = (i + 1)*n + j + 1;
			a++;
		}
	}
	memcpy(pre, data, sizeof(GLfloat)*count * 8);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count * 8, data, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(void *)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(void *)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * numVertices, indices, GL_STATIC_DRAW);
}

void WaterRenderer::draw(
	glm::mat4 projection,
	glm::mat4 view,
	glm::vec3 viewPos,
	glm::vec3 pos,
	glm::vec3 scale,
	glm::vec3 rotAxis,
	GLfloat rotate,
	glm::vec4 color,
	glm::vec3 collisionSize
	) {
	GLfloat* temp = new GLfloat[8 * n*m];
	memcpy(temp, data, sizeof(GLfloat) * 8 * n*m);
	for (int i = 1; i < m - 1; i++) {
		for (int j = 1; j < n - 1; j++) {
			temp[(i*n + j) * 8 + 2] = k1 * data[(i*n + j) * 8 + 2] + k2 * pre[(i*n + j) * 8 + 2] +
				k3 * (data[(i*n + j + 1) * 8 + 2] + data[(i*n + j - 1) * 8 + 2] +
					data[((i + 1)*n + j + 1) * 8 + 2] + data[((i - 1)*n + j + 1) * 8 + 2]);
			if (temp[(i*n + j) * 8 + 2] > 0) temp[(i*n + j) * 8 + 2] = 0;
		}
	}
	memcpy(pre, data, sizeof(GLfloat) * 8 * n*m);
	memcpy(data, temp, sizeof(GLfloat) * 8 * n*m);
	delete[] temp;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * n*m * 8, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * numVertices, indices, GL_STATIC_DRAW);
	Renderer::draw(projection, view, viewPos, pos, scale, rotAxis, rotate, color, collisionSize);
}