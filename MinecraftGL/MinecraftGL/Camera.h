#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

class Camera
{
public:
	Camera(); 

	void setProjection(const glm::mat4& _projection);
	void setModel(const glm::mat4& _model);

	glm::mat4 getProjection() const;
	glm::mat4 getModel() const;
	glm::mat4 getView() const;

	glm::mat4 lookAt();

	void moure(float deltaTime, GLFWwindow* window);
	void girar(GLFWwindow* window);

	glm::vec3 obtPos() const;
	
private:
	void moureDreta(float deltaTime);
	void moureEsquerra(float deltaTime);
	void moureDavant(float deltaTime);
	void moureDarrera(float deltaTime);
	void moureAmunt(float deltaTime);
	void moureAvall(float deltaTime);

	void mirar();

	float velocitat = 2.5f;
	const float sensibilitat = 0.1f;
	float lastX, lastY;

	float yaw = -90.0f;
	float pitch = 0.0f;

	float altura = 2.0f;

	bool primerCop = true;

	glm::vec3 pos;
	glm::vec3 objectiu;
	glm::vec3 direccio; // El vector apunta cap a la càmera
	glm::vec3 cameraUp;
	glm::vec3 right;
	glm::vec3 front;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 model;

};

