#pragma once
//#include "Renderer/Textura.h"
#include "Shaders/ShaderProgram.h"
#include "Recursos.h"
#define Y 120

class Nuvols {
public:
	Nuvols(const glm::mat4& _projection);
	~Nuvols();
	// Renderitza els nuvols
	void render(const glm::mat4& view);
	// Actualitza la textura dels nuvols
	void update(const glm::vec2& pos, float deltaTime);

	void canviaHora(bool _nit);
private:
	void initRenderData();

	float velocitat = 0.075;
	float altura = 200;
	float tamany = 25;

	glm::vec2 offset;
	glm::vec2 pos;
	glm::mat4 projection;
	bool nit = false;

	Textura* t;
	ShaderProgram* shader;

	unsigned int VAOnuvols, VBO;
};