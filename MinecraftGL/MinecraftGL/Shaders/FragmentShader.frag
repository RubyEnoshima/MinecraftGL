#version 330 core

out vec4 color;

in vec3 vertexColor;
in vec2 TexCoord;
flat in int offsetX;
flat in int offsetY;

uniform vec4 inColor;
uniform sampler2D textura;

void main()
{
	vec2 posTex = vec2(TexCoord.x+(offsetX/16.0),TexCoord.y+(offsetY/16.0));
	color = texture(textura, posTex) * vec4(vertexColor, 1.0);
	//color = vec4(vertexColor, 1.0);
}