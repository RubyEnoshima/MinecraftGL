#version 330 core

in vec2 TexCoords;
in float Brillantor;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

uniform vec2 posicioSprite;
uniform vec2 tamanyTextura;
const vec2 tamanySprite = vec2(16);

void main()
{
	vec2 posTex = TexCoords * (tamanySprite / tamanyTextura) + (posicioSprite / tamanyTextura);
	vec4 colorText = texture(image, posTex) * vec4(Brillantor,Brillantor,Brillantor,1.0f);

	if(colorText.a == 0) discard;

	color = vec4(spriteColor) * colorText;
}