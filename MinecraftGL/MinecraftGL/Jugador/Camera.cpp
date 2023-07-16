#include "Camera.h"

Camera::Camera()
{
	//pos = glm::vec3(X * SIZE / 2 + 0.5, Y / 2 + altura, Z * SIZE / 2 + 0.5);
	pos = glm::vec3(0, 65 + altura, 0);
	altura = Y / 2 + altura;
	objectiu = glm::vec3(0.0, 0.0f, -1.0f);
	front = glm::normalize(pos - objectiu);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(front,cameraUp));

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
	right = glm::normalize(glm::cross(front,glm::vec3(0.0f, 1.0f, 0.0f)));
	//cameraUp = glm::normalize(glm::cross(front, right));
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
	glm::mat4 novaView = glm::lookAt(pos, pos + front, cameraUp);
	if (view == novaView) return view;
	view = novaView;
	actualitzaPlans();
	return view;
}

glm::mat4 Camera::mvp() const
{
	return projection*view;
}

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
	//pos.y = altura;
}

void Camera::moureDarrera(float deltaTime, float vel)
{
	pos -= vel * deltaTime * glm::normalize(glm::vec3(front.x * 2, 0, front.z * 2));
	//pos.y = altura;

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

void Camera::teletransporta(const glm::vec3& posNova)
{
	pos = posNova;
}

void Camera::actualitzaPlans()
{
	plansFrustum.clear();

	const float halfVSide = far * tanf(fov * 0.5f);
	const float halfHSide = halfVSide * aspect;
	const glm::vec3 frontMultFar = far * front;

	plansFrustum.push_back(Pla( pos + near * front, front));
	plansFrustum.push_back(Pla( pos + frontMultFar, -front));
	plansFrustum.push_back(Pla( pos, glm::cross(frontMultFar - right * halfHSide, cameraUp)));
	plansFrustum.push_back(Pla( pos, glm::cross(cameraUp,frontMultFar + right * halfHSide)));
	plansFrustum.push_back(Pla( pos, glm::cross(right, frontMultFar - cameraUp * halfVSide)));
	plansFrustum.push_back(Pla( pos, glm::cross(frontMultFar + cameraUp * halfVSide, right)));

}

vector<Pla> Camera::obtPlans() const
{
	return plansFrustum;
}
