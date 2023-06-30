#pragma once
#include "Renderer/Textura.h"
#include "Shaders/ShaderProgram.h"

#define Y 120

class Nuvols {
public:
	Nuvols();
	~Nuvols();
	// Renderitza els nuvols
	void render(const glm::mat4& view, const glm::mat4& projection);
	// Actualitza la textura dels nuvols
	void update();
private:
	void initRenderData();

	float velocitat = 0.01;
	float tamany = 16;
	glm::vec2 offset;

	Textura* t;
	ShaderProgram* shader;

	unsigned int VAOnuvols, VBO;
};