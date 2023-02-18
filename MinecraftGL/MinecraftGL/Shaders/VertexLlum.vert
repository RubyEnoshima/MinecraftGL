#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 aOffset;

// Iluminació
out vec3 normals;
out vec3 FragPos;

// Textura
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
	normals = mat3(transpose(inverse(model))) * aNormals;
	FragPos = vec3(model * vec4(aPos, 1.0));
}