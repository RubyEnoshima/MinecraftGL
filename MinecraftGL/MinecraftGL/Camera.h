#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	void moure();
private:
	glm::vec3 pos;
	glm::vec3 objectiu;
	glm::vec3 direccio; // El vector apunta cap a la càmera
	glm::vec3 cameraUp;
	glm::vec3 right;
};

