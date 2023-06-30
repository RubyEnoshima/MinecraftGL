#version 330 core

const vec4 colorNuvols = vec4(1.0f);

in vec2 TexCoord;
out vec4 color;

uniform sampler2D uTexture;

uniform vec2 posicioSprite;
uniform vec2 tamanyTextura;
uniform vec2 tamanySprite;

void main()
{
    vec2 posTex = TexCoord * (tamanySprite / tamanyTextura) + (posicioSprite / tamanyTextura);
    vec4 colorText = texture(uTexture, posTex);

	if(colorText.a == 0) discard;

	color = colorNuvols * colorText;
}