#version 330 core

out vec4 color;

// Iluminació
//in vec3 normals;
//in vec3 FragPos;
in float llum;

// Textura
in vec2 TexCoord;
flat in int offsetX;
flat in int offsetY;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D textura;

void main()
{
	// Textura
	vec2 posTex = vec2(TexCoord.x+(offsetX/16.0),TexCoord.y+(offsetY/16.0));
	vec4 colorText = texture(textura, posTex);

	// Funciona per cristal, pero es pot clicar un bloc a través...
	if(colorText.a == 0) discard;

	// Ambient
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	//vec3 norm = normalize(normals);
	//vec3 lightDir = normalize(lightPos - FragPos);
	//float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = diff * lightColor;

	color = vec4( (ambient + pow(5 / 16f, 1.4f)) * colorText.xyz, colorText.w);
}