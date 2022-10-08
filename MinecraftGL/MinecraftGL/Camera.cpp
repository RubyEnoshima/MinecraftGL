#include "Camera.h"

void Camera::mirar() {
	glm::vec3 nouFront;
	nouFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	nouFront.y = sin(glm::radians(pitch));
	nouFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(nouFront);
}

void Camera::girar(GLFWwindow* window) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (primerCop) {
		lastX = xpos;
		lastY = ypos;
		primerCop = false;
	}
	
	float nouYaw = xpos - lastX, nouPitch = lastY - ypos;

	if (nouYaw == 0 and nouPitch == 0) return; // Estalviem temps en mirar què canvia si no hi ha res a fer

	cout << "Yaw: " << yaw << endl;

	lastX = xpos; lastY = ypos;

	nouYaw *= sensibilitat;
	nouPitch *= sensibilitat;

	yaw += nouYaw;
	pitch += nouPitch;

	// El mantenim sempre entre 90 i -90 perquè no surti
	if (pitch > 89.0f) pitch = 89.0f;
	else if (pitch < -89.0f) pitch = -89.0f;

	mirar();
}

Camera::Camera(float w,float h)
{
	pos = glm::vec3(0.0f, 0.0f, 3.0f);
	objectiu = glm::vec3(0.0, 0.0f, 0.0f);
	direccio = glm::normalize(pos - objectiu);
	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direccio));
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mirar();
}

glm::mat4 Camera::lookAt()
{
	return glm::lookAt(pos, pos + front, cameraUp);
}

void Camera::moure(float deltaTime, GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W)) {
		moureDavant(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		moureDarrera(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		moureEsquerra(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		moureDreta(deltaTime);
	}
}

void Camera::moureDreta(float deltaTime)
{
	pos += glm::normalize(glm::cross(front, cameraUp)) * velocitat * deltaTime;
}

void Camera::moureEsquerra(float deltaTime)
{
	pos -= glm::normalize(glm::cross(front, cameraUp)) * velocitat * deltaTime;
}

void Camera::moureDavant(float deltaTime)
{
	pos += velocitat * deltaTime * front;
	pos.y = 0.0f;
}

void Camera::moureDarrera(float deltaTime)
{
	pos -= velocitat * deltaTime * front;
	pos.y = 0.0f;

}
