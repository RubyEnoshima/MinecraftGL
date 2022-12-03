#version 330 core

out vec4 color;

in vec3 vertexColor;
in vec2 TexCoord;

uniform vec4 inColor;
uniform sampler2D textura;

void main()
{
	color = texture(textura, TexCoord) * vec4(vertexColor, 1.0);
	//color = vec4(vertexColor, 1.0);
}