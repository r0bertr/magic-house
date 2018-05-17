#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = tex;
    gl_Position = projection * view * model * vec4(pos, 1.f);
}