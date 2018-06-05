#version 330 core
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture0;  // diffuse
uniform sampler2D texture1;  // specular
uniform sampler2D texture2;  // shininess
uniform sampler2D texture3;  // depth

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform vec4 color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	fragColor = vec4(result, 1.0);
    // fragColor = texture(texture0, texCoords);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture0, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture0, texCoords));
    vec3 specular = light.specular * spec; // needed texture1 for specular
    return (ambient + diffuse + specular);
}