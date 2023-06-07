#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

uniform vec2 posicioSprite;
uniform vec2 tamanyTextura;
uniform vec2 tamanySprite;

void main()
{
	vec2 posTex = TexCoords * (tamanySprite / tamanyTextura) + (posicioSprite / tamanyTextura);
	vec4 colorText = texture(image, posTex);

	if(colorText.a == 0) discard;

	color = vec4(spriteColor) * colorText;
}