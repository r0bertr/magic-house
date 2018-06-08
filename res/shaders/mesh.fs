#version 330 core
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform bool hasTexture;
uniform sampler2D texture0;  // diffuse
uniform sampler2D texture1;  // specular
uniform sampler2D texture2;  // shininess
uniform sampler2D texture3;  // depth

uniform vec3 viewPos;
uniform Light light;
uniform vec4 color;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = CalcDirLight(norm, viewDir);
	fragColor = vec4(result, 1.f);
    // fragColor = texture(texture0, texCoords);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = vec3(0.f);
    if (light.direction == vec3(0.f)) {
        lightDir = normalize(light.position - fragPos);
    } else {
        lightDir = normalize(-light.direction);
    }
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.f);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    if (hasTexture) {
        return (ambient + diffuse + specular) * 
            texture(texture0, texCoords).rgb * color.xyz;
    } else {
        return (ambient + diffuse + specular) * color.xyz;
    }
}
