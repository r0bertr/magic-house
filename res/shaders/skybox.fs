#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube texture0;
uniform float alpha;

void main()
{
    //vec4 textureColor = texture(texture0, TexCoords);
	// textureColor.a = alpha;
	//textureColor = textureColor * alpha;
	//FragColor = textureColor;
	FragColor = vec4(texture(texture0, TexCoords).rgb, alpha);
}