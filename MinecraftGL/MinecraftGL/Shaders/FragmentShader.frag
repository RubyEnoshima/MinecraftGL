#version 330 core

out vec4 color;

// Iluminació
in float llumArtificial;
in float llumNatural;

// Textura
in vec2 TexCoord;
flat in int offsetX;
flat in int offsetY;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D textura;
uniform sampler2D lightMap;

void main()
{
	// Textura
	vec2 posTex = vec2(TexCoord.x+(offsetX/16.0),TexCoord.y+(offsetY/16.0));
	vec4 colorText = texture(textura, posTex);

	// Funciona per cristal, pero es pot clicar un bloc a través...
	if(colorText.a == 0) discard;


	// Ambient, per tal que la foscor no sigui tan fosca
	float ambientStrength = 0.15;
	vec3 ambient = ambientStrength * lightColor;

	// Iluminació
	float gamma = 0.95 / 0.75;

	// Artificial
	float resArtificial = pow(llumArtificial / 16f, gamma);
	float resNatural = pow(llumNatural / 16f, gamma);
	vec3 colorLlum = vec3(1.0,1.0,0.75);
	vec3 LlumFinal = colorLlum*max(resArtificial,resNatural);

	// Natural
	//vec2 posLight = vec2(TexCoord.x+(0.0/16.0),TexCoord.y+(llumArtificial/16.0));

	color = vec4( (ambient + LlumFinal) * colorText.xyz, colorText.w);

	//vec4 colorLight = texture(lightMap, posLight);
//	color = colorText * vec4(colorLight.xyz * gamma, colorLight.a);
	//color = texture(lightMap, posTex);
}