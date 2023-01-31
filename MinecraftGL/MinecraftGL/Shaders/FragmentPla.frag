#version 330 core

out vec4 color;

in vec3 vertexColor;

uniform vec4 inColor;

void main()
{
	color = vec4(vertexColor, 1.0);
}