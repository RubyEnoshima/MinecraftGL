#pragma once
#include "Joc.h"
#include "glm/gtx/io.hpp"

class RayCast
{
public:
	RayCast(Camera* cam, Renderer* renderer);

	glm::vec3 calcularRay();

	bool intersecta(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2) const;

	float calcT(const glm::vec3& O, const glm::vec3& D, const glm::vec3 n) const;
	glm::vec3 calcPunt(const glm::vec3& O, const glm::vec3& D, float t) const;

private:
	glm::vec3 Dir;
	Camera* camera;
	Renderer* r;
};

