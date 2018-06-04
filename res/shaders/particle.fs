#version 330 core
in vec2 texCoords;
in vec3 normal;
out vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;
uniform sampler2D texture8;
uniform sampler2D texture9;
uniform sampler2D texture10;
uniform sampler2D texture11;
uniform sampler2D texture12;
uniform sampler2D texture13;
uniform sampler2D texture14;
uniform sampler2D texture15;

uniform vec4 color;

void main() {
    vec4 textureColor1 = texture(texture0, texCoords);
    if (textureColor1.a < 0.1f)
        discard;
    fragColor = textureColor1 * color;
}