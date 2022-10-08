#include "Camera.h"

Camera::Camera()
{
	pos = glm::vec3(0.0f, 1.7f, 3.0f);

	objectiu = glm::vec3(0.0, 0.0f, 0.0f);

	direccio = glm::normalize(pos - objectiu);

	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direccio));

	cameraUp = glm::cross(direccio, right);
}

glm::mat4 Camera::moure(float x, float z)
{
	return glm::lookAt(pos, objectiu, cameraUp);
}

void Camera::moureDreta()
{
	pos += glm::normalize(glm::cross(direccio, cameraUp)) * velocitat;
}

void Camera::moureEsquerra()
{
	pos -= glm::normalize(glm::cross(direccio, cameraUp)) * velocitat;
}

void Camera::moureDavant()
{
	pos += velocitat * direccio;
}

void Camera::moureDarrera()
{
	pos -= velocitat * direccio;
}
