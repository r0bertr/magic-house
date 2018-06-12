#version 330 core
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLightSpace;
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

vec3 CalcDirLight(vec3 norm, vec3 viewDir, vec4 fragPosLightSpace);

void main() {
	vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    
	vec3 result = CalcDirLight(norm, viewDir, fragPosLightSpace);
	fragColor = vec4(result, 1.f);
    // fragColor = texture(texture0, texCoords);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(vec3 norm, vec3 viewDir, vec4 fragPosLightSpace)
{
    vec3 lightDir = vec3(0.f);
    if (light.direction == vec3(0.f)) {
        lightDir = normalize(light.position - fragPos);
    } else {
        lightDir = normalize(-light.direction);
    }
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.f);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    
    // Shadow
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(texture3, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = .0005f;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(texture3, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(texture3, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if (hasTexture) {
        return (ambient + (1-shadow) * (diffuse + specular)) * 
            texture(texture0, texCoords).rgb * color.xyz;
    } else {
        return (ambient + (1-shadow) * (diffuse + specular)) * color.xyz;
    }
}