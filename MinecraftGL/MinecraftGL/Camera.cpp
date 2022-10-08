#include "Camera.h"

Camera::Camera()
{
	pos = glm::vec3(0.0f, 0.0f, 3.0f);
	objectiu = glm::vec3(0.0, 0.0f, 0.0f);
	direccio = glm::normalize(pos - objectiu);
	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direccio));
	cameraUp = glm::cross(direccio, right);
}
