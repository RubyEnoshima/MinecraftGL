#pragma once
#include "Joc.h"

class RayCast
{
public:
	RayCast(Camera* cam, Renderer* renderer, GLFWwindow* _window);

	glm::vec3 calcularRay() const;
	glm::vec3 calcularRay2() const;

	bool intersecta(const glm::vec3& O, const glm::vec3& D, const glm::vec3 n) const;

	float calcT(const glm::vec3& O, const glm::vec3& D, const glm::vec3 n) const;
	glm::vec3 calcPunt(const glm::vec3& O, const glm::vec3& D, float t) const;

private:
	Camera* camera;
	GLFWwindow* window;
	Renderer* r;
};

