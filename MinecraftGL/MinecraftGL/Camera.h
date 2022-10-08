#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

class Camera
{
public:
	Camera(); // Constructor

	glm::mat4 moure(float x, float z);

	void moureDreta();
	void moureEsquerra();
	void moureDavant();
	void moureDarrera();
private:
	const float velocitat = 0.05f;

	glm::vec3 pos;
	glm::vec3 objectiu;
	glm::vec3 direccio; // El vector apunta cap a la càmera
	glm::vec3 cameraUp;
	glm::vec3 right;
};

