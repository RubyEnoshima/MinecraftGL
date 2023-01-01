#pragma once

#include "Chunk.h"
#include "SuperChunk.h"
#include "Renderer.h"
#include "Camera.h"
#include "RayCast.h"
#include "glm/gtx/io.hpp"

#define rgb renderer.rgb

#include "Cub.h"
using namespace std;

class Joc
{
public:
	void gameLoop(); // Fa que comenci el joc i executa el loop

	int crearFinestra(); // Retorna 1 si s'ha pogut crear la finestra

	// Funcions per quan es prem alguna tecla o botó
	void DestruirCub();
	void PosarCub();
	
	
private:
	void ObtenirCubMira(); // Pos en CubActual el vector del cub al que estem mirant
	glm::vec3 ObtenirCostat() const;

	void canviarModeMouse(int mode);

	void loop();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	
	GLFWwindow* window;
	glm::vec3 CubActual;
	glm::vec3 Costat;

	Camera camera;
	Renderer renderer;
	SuperChunk* mon;
};

