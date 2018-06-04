#version 330 core
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

struct Material {
    vec3 specular;    
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture0; // diffuse map
uniform sampler2D texture1; // specular map
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

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	fragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture0, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture0, texCoords));
    vec3 specular = light.specular * spec * material.specular; // needed texture1 for specular
    return (ambient + diffuse + specular);
}