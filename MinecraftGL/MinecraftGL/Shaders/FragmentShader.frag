#version 330 core

out vec4 color;

// Iluminació
in vec3 normals;
in vec3 FragPos;

// Textura
in vec2 TexCoord;
flat in int offsetX;
flat in int offsetY;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D textura;

void main()
{
	// Ambient
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 norm = normalize(normals);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec2 posTex = vec2(TexCoord.x+(offsetX/16.0),TexCoord.y+(offsetY/16.0));
	color = vec4( (ambient + diffuse) * vec3( texture(textura, posTex).xyz ), 1.0);
}