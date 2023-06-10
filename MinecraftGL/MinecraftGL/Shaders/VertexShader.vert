#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormals;
layout (location = 1) in float aLlum;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 aOffset;
layout (location = 4) in float aCostat;
layout (location = 5) in vec3 aColor;

// Iluminació
//out vec3 normals;
//out vec3 FragPos;
out float llumArtificial;
out float llumNatural;

// Textura
out vec2 TexCoord;
flat out int offsetX;
flat out int offsetY;

flat out int costat;

out vec3 colorTint;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	// Posició
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	//vertexColor = color;

	// Textura
	TexCoord = aTexCoord/16;
	offsetX = int(aOffset.x);
	offsetY = int(aOffset.y);

	// Iluminació
	llumArtificial = int(aLlum) & 0x0F;
	llumNatural = (int(aLlum) >> 4) & 0xF;

	costat = int(aCostat);
	colorTint = aColor;
}