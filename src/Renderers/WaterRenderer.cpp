#include "lib/glad.h"
#include "Renderers/WaterRenderer.hpp"

#include <cstdio>
#include <vector>

WaterRenderer::WaterRenderer(Shader *shader, Texture **textures,
	Light *light) : Renderer(shader, textures, light) {
	initRenderData();
}

WaterRenderer::~WaterRenderer() {
	if (data != NULL) delete[] data;
	if (pre != NULL) delete[] pre;
	if (height != NULL) delete[] height;
}

void koch(std::vector<glm::vec2> &offset, glm::vec2 begin, glm::vec2 end, int iteration, int pos) {
	if (iteration < 0) return;
	float angle = 3.141592653 / 3;
	glm::vec2 diff = 0.125f * (end - begin);
	glm::vec2 temp;
	glm::vec2 first = begin + 2.0f * diff;
	temp.x = diff.x*cos(angle) - diff.y*sin(angle);
	temp.y = diff.x*sin(angle) + diff.y*cos(angle);
	glm::vec2 second = first + 2.0f * temp;
	temp.x = diff.x*cos(angle) + diff.y*sin(angle);
	temp.y = -diff.x*sin(angle) + diff.y*cos(angle);
	glm::vec2 third = second + 4.0f*temp;
	temp.x = diff.x*cos(angle) - diff.y*sin(angle);
	temp.y = diff.x*sin(angle) + diff.y*cos(angle);
	glm::vec2 forth = third + 2.0f*temp;
	int step = pow(5, iteration);
	offset[pos] = begin;
	offset[pos + 1 * step] = first;
	offset[pos + 2 * step] = second;
	offset[pos + 3 * step] = third;
	offset[pos + 4 * step] = forth;
	offset[pos + 5 * step] = end;
	koch(offset, begin, first, iteration - 1, pos);
	koch(offset, first, second, iteration - 1, pos + step);
	koch(offset, second, third, iteration - 1, pos + 2 * step);
	koch(offset, third, forth, iteration - 1, pos + 3 * step);
	koch(offset, forth, end, iteration - 1, pos + 4 * step);
}

void WaterRenderer::initRenderData() {
	n = 50; m = 626;
	std::vector<glm::vec2> offset(m);
	for (int i = 0; i < m; i++) {
		offset[i] = glm::vec2(i, 0);
	}
	koch(offset, glm::vec2(0, 0), glm::vec2(m - 1, 0), 3, 0);
	d = 1.0f; t = 1.f; c = 0.01f; mu = 0.f;
	int count = n*m;
	float f1 = c * c * t * t / (d * d);
	float f2 = 1.0f / (mu * t + 2);
	float k1 = (4.0f - 8.0f * f1) * f2;
	float k2 = (mu * t - 2) * f2;
	float k3 = 2.0f * f1 * f2;

	numVertices = 6 * (m - 1) * (n - 1);
	data = new GLfloat[count * 8];
	height = new GLfloat[count];
	pre = new GLfloat[count];
	indices = new GLint[numVertices];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			data[(i*n + j) * 8] = (i + 0.05 * offset[i].x) * d;
			data[(i*n + j) * 8 + 1] = (j + 0.05 * offset[i].y) * d;
			if (i == 0 || j == 0 || i == m - 1 || j == n - 1) {
				height[i*n + j] = 0.f;
			}
			else {
				int r = rand();
				if (r % 2 == 0) {
					height[i*n + j] = -1.f;
				}
				else {
					height[i*n + j] = 0.f;
				}
			}
			data[(i*n + j) * 8 + 2] = height[i*n + j];
			data[(i*n + j) * 8 + 3] = 0.1f * (i % 2);
			data[(i*n + j) * 8 + 4] = 0.1f * (1 - j % 2);
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
	memcpy(pre, height, sizeof(GLfloat)*count);
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
	GLfloat* temp = new GLfloat[n*m];
	memcpy(temp, height, sizeof(GLfloat) * n*m);

	t = 1.f;
	float f1 = c * c * t * t / (d * d);
	float f2 = 1.0f / (mu * t + 2);
	float k1 = (4.0f - 8.0f * f1) * f2;
	float k2 = (mu * t - 2) * f2;
	float k3 = 2.0f * f1 * f2;
	
	for (int i = 1; i < m - 1; i++) {
		for (int j = 1; j < n - 1; j++) {
			temp[i*n + j] = k1 * height[i*n + j] + k2 * pre[i*n + j] +
				k3 * (height[i*n + j + 1] + height[i*n + j - 1] + height[(i + 1)*n + j] + height[(i - 1)*n + j]);
			if (temp[i*n + j] > 0) {
				data[(i*n + j) * 8 + 2] = -0.01f;
			}
			else {
				data[(i*n + j) * 8 + 2] = temp[i*n + j];
			}
		}
	}
	memcpy(pre, height, sizeof(GLfloat) * n*m);
	memcpy(height, temp, sizeof(GLfloat) * n*m);
	delete[] temp;
	for (int i = 1; i < m - 1; i++) {
		for (int j = 1; j < n - 1; j++) {
			data[(i*n + j) * 8 + 5] = data[(i*n + j + 1) * 8 + 2] - data[(i*n + j - 1) * 8 + 2];
			data[(i*n + j) * 8 + 6] = data[((i + 1)*n + j) * 8 + 2] - data[((i - 1)*n + j) * 8 + 2];
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * n*m * 8, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * numVertices, indices, GL_STATIC_DRAW);
	Renderer::draw(projection, view, viewPos, pos, scale, rotAxis, rotate, color, collisionSize);
}