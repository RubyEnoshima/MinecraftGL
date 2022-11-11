#version 330 core

out vec4 color;
in vec4 vertexColor;

uniform vec4 inColor;

void main()
{
	color = vertexColor;
}