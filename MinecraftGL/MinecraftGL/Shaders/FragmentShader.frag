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

uniform bool bounding;

float tamanyAlfaX = 16.0;
float tamanyAlfaY = 16.0;

void main()
{
	if(bounding){
		color = vec4(0,0,0,1);
		return;
	}

	// Textura
	vec2 posTex = vec2(TexCoord.x+(offsetX/tamanyAlfaX),TexCoord.y+(offsetY/tamanyAlfaY));
	vec4 colorText = texture(textura, posTex);

	// Funciona per cristal, pero es pot clicar un bloc a través...
	if(colorText.a == 0) discard;


	// Ambient, per tal que la foscor no sigui tan fosca
	float ambientStrength = 0.12;
	vec3 ambient = ambientStrength * lightColor;

	// Iluminació
	float gamma = 0.95 / 0.75;

	float resArtificial = pow(llumArtificial / 15f, gamma);

	float intensitatNatural = 0.95; // FER-HO UNIFORM
	float resNatural = pow(llumNatural / 15f, gamma) * intensitatNatural;

	vec3 colorLlum = vec3(0.95,0.95,0.95);
	if(resArtificial > resNatural) colorLlum = vec3(1.0,1.0,0.87);

	float suma = resArtificial+resNatural;
	if(suma > 1) suma = 1;
	vec3 LlumFinal = colorLlum*suma*0.9;

	vec4 brillantor = vec4(0.95,0.95,0.95,1);

	// El resultat final és la suma de l'ambient i la llum calculada abans, amb un percentatge segons la cara, pel color obtingut de la textura
	color = vec4( (ambient + LlumFinal) * colorText.xyz, colorText.w)*brillantor;

}