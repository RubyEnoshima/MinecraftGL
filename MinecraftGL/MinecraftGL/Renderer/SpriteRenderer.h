#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Shaders/ShaderProgram.h"
#include "Textura.h"
class SpriteRenderer
{
public:
	SpriteRenderer(ShaderProgram* _shader, Renderer* _renderer);
	~SpriteRenderer();
	void DrawSprite(Textura& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(1.0f, 1.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	void initRenderData();

	Renderer* renderer;
	ShaderProgram* shader;
	unsigned int quadVAO;
};

