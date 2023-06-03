#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders/ShaderProgram.h"
#include "Framebuffer.h"
#include "Textura.h"
class SpriteRenderer
{
public:
	SpriteRenderer(const ShaderProgram& _shader);
	~SpriteRenderer();
	void DrawSprite(Textura& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	ShaderProgram shader;
	unsigned int quadVAO;
	void initRenderData();
};

