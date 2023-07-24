#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mon/SuperChunk.h"

#include <iostream>
using namespace std;

#define ALTURA_JUG 1

class Camera
{
public:
	Camera(); 

	void setProjection(float _fov, float _aspectRatio, float _near, float _far);
	void setModel(const glm::mat4& _model);

	glm::mat4 getProjection() const;
	glm::mat4 getModel() const;
	glm::mat4 getView() const;

	// Retorna la posició de la càmera
	glm::vec3 obtPos() const;
	// Retorna cap a on mira la càmera
	glm::vec3 obtDireccio() const;

	glm::vec3 obtRight() const;

	glm::mat4 lookAt();
	glm::mat4 mvp() const;

	void girar(GLFWwindow* window);

	void moureDreta(float deltaTime, float vel);
	void moureEsquerra(float deltaTime, float vel);
	void moureDavant(float deltaTime, float vel);
	void moureDarrera(float deltaTime, float vel);
	void moureAmunt(float deltaTime, float vel);
	void moureAvall(float deltaTime, float vel);

	void teletransporta(const glm::vec3& posNova);
	
	void actualitzaPlans();
	vector<Pla> obtPlans() const;
private:
	vector<Pla> plansFrustum;
	const float sensibilitat = 0.1f;

	void mirar();
	float lastX, lastY;

	float yaw = -90.0f;
	float pitch = 0.0f;

	float altura = ALTURA_JUG;

	bool primerCop = true;

	glm::vec3 pos;
	glm::vec3 objectiu;
	glm::vec3 cameraUp;
	glm::vec3 right;
	glm::vec3 front;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 model;

	float fov;
	float aspect;
	float near, far;
};

