#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 color;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 aOffset;

//out vec3 vertexColor;

out vec2 TexCoord;
flat out int offsetX;
flat out int offsetY;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	//vertexColor = color;
	TexCoord = aTexCoord/16;
	offsetX = int(aOffset.x);
	offsetY = int(aOffset.y);
}