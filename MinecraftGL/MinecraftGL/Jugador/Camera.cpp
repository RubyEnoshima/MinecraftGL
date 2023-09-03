#include "Camera.h"

Camera::Camera()
{
	pos = glm::vec3(0, 90, 0);

	mirar();
}

void Camera::setProjection(float _aspectRatio)
{
	projection = glm::perspective(fov, _aspectRatio, near, far);
	aspect = _aspectRatio;
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
	right = glm::normalize(glm::cross(front,glm::vec3(0,1,0)));
	cameraUp = glm::normalize(glm::cross(right, front));
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

glm::vec3 Camera::obtRight() const
{
	return right;
}

glm::mat4 Camera::lookAt()
{
	glm::mat4 novaView = glm::lookAt(pos, pos + front, cameraUp);
	if (view == novaView) return view;
	view = novaView;
	actualitzaPlans();
	return view;
}

void Camera::teletransporta(const glm::vec3& posNova)
{
	pos = posNova;
}

// Tret de https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
void Camera::actualitzaPlans()
{	
	const float halfVSide = far * tanf(fov * .5f);
	const float halfHSide = halfVSide * aspect;
	const glm::vec3 frontMultFar = far * front;

	frustum.nearFace = Pla(pos + near * front, glm::normalize(front));
	frustum.farFace = Pla(pos + frontMultFar, glm::normalize(-front));
	frustum.rightFace = Pla(pos, glm::normalize(glm::cross(frontMultFar - right * halfHSide, cameraUp)) );
	frustum.leftFace = Pla(pos, glm::normalize(glm::cross(cameraUp, frontMultFar + right * halfHSide)) );
	frustum.topFace = Pla(pos, glm::normalize(glm::cross(right, frontMultFar - cameraUp * halfVSide)) );
	frustum.bottomFace = Pla(pos, glm::normalize(glm::cross(frontMultFar + cameraUp * halfVSide, right)) );
}
