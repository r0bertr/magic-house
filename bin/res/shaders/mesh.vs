#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec3 inNormal;

out vec3 fragPos;
out vec2 texCoords;
out vec3 normal;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
	fragPos = vec3(model * vec4(inPos, 1.0));
    texCoords = inTexCoords;
    normal = mat3(transpose(inverse(model))) * inNormal;
    gl_Position = projection * view * model * vec4(inPos, 1.f);
    fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.f);
}