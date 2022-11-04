#pragma once
#include "Joc.h"

class RayCast
{
public:
	RayCast(Camera* cam, Renderer* renderer, GLFWwindow* _window);

	glm::vec3 calcularRay() const;

	bool rayTriangleIntersect(
		const glm::vec3& orig, const glm::vec3& dir,
		const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
		float& t);

private:
	Camera* camera;
	GLFWwindow* window;
	Renderer* r;
};

