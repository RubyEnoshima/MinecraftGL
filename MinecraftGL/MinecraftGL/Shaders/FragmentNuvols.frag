#version 330 core

const float densitat = .00005;
const float distanciaFog = 14;
vec4 colorNuvols = vec4(1.0f);
vec4 fogcolor = vec4(0.7, 0.8, 1.0, 1.0);

in vec2 TexCoord;
out vec4 color;

uniform sampler2D image;

uniform vec2 posicioSprite;
uniform vec2 tamanyTextura;
uniform vec2 tamanySprite;

uniform bool nit;

float calculaNiebla(float distancia){
	float fogFactor = exp(5-densitat * distancia * distancia);
    return clamp(fogFactor, 0.0f, 1.0f);
}

void main()
{
    vec2 posTex = TexCoord * (tamanySprite / tamanyTextura) + (posicioSprite / tamanyTextura);
    vec4 colorText = texture(image, posTex);

	if(colorText.a == 0) discard;
	else colorText.a /= 1.5;

	float alfa = colorText.a;

	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fog = calculaNiebla(z);

	if(nit){
		colorNuvols *= 0.1;
		fogcolor *= 0.01;
	}

	color = colorNuvols * colorText;
	color = mix(fogcolor, color, fog);
	color.a = alfa;
}