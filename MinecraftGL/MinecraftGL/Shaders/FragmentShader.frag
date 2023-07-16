#version 330 core

const vec4 fogcolor = vec4(0.7, 0.8, 1.0, 1.0);
const float densitat = .0005;
const float distanciaFog = 7;

in vec3 FragPos;
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
//uniform sampler2D lightMap;

uniform bool bounding;

float tamanyMapaX = 16.0;
float tamanyMapaY = 16.0;

flat in int costat;

in vec3 colorTint;

// Ens diu el factor que s'ha d'utilitzar per la boira (0: tot boira, 1: sense boira)
float calculaNiebla(float distancia){
	float fogFactor = exp(distanciaFog-densitat * distancia * distancia);
    return clamp(fogFactor, 0.0f, 1.0f);
}

void main()
{
	if(bounding){
		color = vec4(0,0,0,1);
		return;
	}

	// Textura
	vec2 posTex = vec2(TexCoord.x+(offsetX/tamanyMapaX),TexCoord.y+(offsetY/tamanyMapaY));
	vec4 colorText = texture(textura, posTex);

	// Funciona per cristal, pero es pot clicar un bloc a través...
	if(colorText.a == 0) discard;

	// Calculem la distancia del fragment a la pantalla
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fog = calculaNiebla(z);

	// Si la quantitat és molt petita, no fa falta renderitzar res
	if(fog <= 0.075) discard;

	// Quanta més boira hi hagi, menys es veurà el fragment.
	float alfa = fog * colorText.a;

	// Ambient, per tal que la foscor no sigui tan fosca
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Iluminació
	float gamma = 0.8 / 0.75;

	float resArtificial = pow(llumArtificial / 15f, gamma);

	float intensitatNatural = 0.9; // FER-HO UNIFORM
	float resNatural = 15;//pow(llumNatural / 15f, gamma) * intensitatNatural;

	vec3 colorLlum = vec3(0.98,0.98,0.98);
	if(resArtificial > resNatural) colorLlum = vec3(1.0,1.0,0.87);

	float suma = resArtificial+resNatural;
	if(suma > 1) suma = 1;
	vec3 LlumFinal = colorLlum*suma*0.9;

	vec4 brillantor = vec4(0.99,0.99,0.99,1);
	vec4 ombres = vec4(1.0,0.75,0.45,0.2);                                                                                                                                                                                                                                                                                                                                                                                                              // UwU

	// El resultat final és la suma de l'ambient i la llum calculada abans, amb un percentatge segons la cara, pel color obtingut de la textura
	color = vec4( (ambient + LlumFinal) * colorText.xyz * ombres[costat] * colorTint, colorText.w)*brillantor;
	
	color = mix(fogcolor, color, fog);
	color.a = alfa;
}