#include "Camera.h"

Camera::Camera()
{
	//pos = glm::vec3(X * SIZE / 2 + 0.5, Y / 2 + altura, Z * SIZE / 2 + 0.5);
	pos = glm::vec3(X/2, Y / 2 + altura, Z/2);
	altura = Y / 2 + altura;
	objectiu = glm::vec3(-1.0, 0.0f, -1.0f);
	direccio = glm::normalize(pos - objectiu);
	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direccio));
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mirar();
}

void Camera::setProjection(float _fov, float _aspectRatio, float _near, float _far)
{
	projection = glm::perspective(_fov, _aspectRatio, _near, _far);
	fov = _fov;
	aspect = _aspectRatio;
	near = _near;
	far = _far;
}

void Camera::setModel(const glm::mat4& _model)
{
	model = _model;
}

glm::mat4 Camera::getProjection() const
{
	return projection;
}

glm::mat4 Camera::getModel() const
{
	return model;
}

glm::mat4 Camera::getView() const
{
	return view;
}

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

glm::vec3 Camera::obtPos() const
{
	return pos;
}

glm::vec3 Camera::obtDireccio() const
{
	return front;
}


glm::mat4 Camera::lookAt()
{
	view = glm::lookAt(pos, pos + front, cameraUp);
	return view;
}

//void Camera::moure(float deltaTime, GLFWwindow* window)
//{
//	// Córrer
//	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
//		velocitatAct = velocitat * 2.0f;
//	}
//	else velocitatAct = velocitat;
//
//	if (glfwGetKey(window, GLFW_KEY_W)) {
//		moureDavant(deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_S)) {
//		moureDarrera(deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_A)) {
//		moureEsquerra(deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_D)) {
//		moureDreta(deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
//		moureAmunt(deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
//		moureAvall(deltaTime);
//	}
//}

void Camera::moureDreta(float deltaTime, float vel)
{
	pos += glm::normalize(glm::cross(front, cameraUp)) * vel * deltaTime;
}

void Camera::moureEsquerra(float deltaTime, float vel)
{
	pos -= glm::normalize(glm::cross(front, cameraUp)) * vel * deltaTime;
}

void Camera::moureDavant(float deltaTime, float vel)
{
	pos += vel * deltaTime * glm::normalize(glm::vec3(front.x * 2, 0, front.z * 2));
	pos.y = altura;
}

void Camera::moureDarrera(float deltaTime, float vel)
{
	pos -= vel * deltaTime * glm::normalize(glm::vec3(front.x * 2, 0, front.z * 2));
	pos.y = altura;

}

void Camera::moureAmunt(float deltaTime, float vel)
{
	pos.y += vel * deltaTime;
	altura = pos.y;
}

void Camera::moureAvall(float deltaTime, float vel)
{
	pos.y -= vel * deltaTime;
	altura = pos.y;

}