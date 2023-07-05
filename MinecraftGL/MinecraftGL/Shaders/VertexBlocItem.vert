#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in float aBrillantor;

out vec2 TexCoords;
out float Brillantor;

uniform mat4 model;

const float escala = 0.85;

void main()
{
	TexCoords = vertex.zw;
	gl_Position = model * vec4(vertex.x * escala + 0.1, vertex.y * escala - 0.9, 0.0, 1.0);
	Brillantor = aBrillantor;
}
