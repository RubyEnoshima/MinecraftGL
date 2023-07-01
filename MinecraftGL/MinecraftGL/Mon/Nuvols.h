#pragma once
#include "Renderer/Textura.h"
#include "Shaders/ShaderProgram.h"

#define Y 120

class Nuvols {
public:
	Nuvols(const glm::mat4& _projection);
	~Nuvols();
	// Renderitza els nuvols
	void render(const glm::mat4& view);
	// Actualitza la textura dels nuvols
	void update(const glm::vec2& pos);
private:
	void initRenderData();

	float velocitat = 0.0001;
	float tamany = 16;
	glm::vec2 offset;
	glm::vec2 pos;

	glm::mat4 projection;

	Textura* t;
	ShaderProgram* shader;

	unsigned int VAOnuvols, VBO;
};