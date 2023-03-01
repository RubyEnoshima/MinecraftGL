#pragma once

#include "Mon/SuperChunk.h"
#include "Renderer/Renderer.h"
#include "Jugador/Camera.h"
#include "glm/gtx/io.hpp"

#include <string>

#define rgb renderer.rgb

using namespace std;

class Joc
{
public:
	~Joc();

	// Fa que comenci el joc i executa el loop
	void gameLoop(); 

	// Retorna 1 si s'ha pogut crear la finestra
	int crearFinestra(); 

	void canviarProjeccio();

	// -- Funcions per quan es prem alguna tecla o botó --

	void DestruirCub();
	void PosarCub();
	
	void Culling();
	void VSync();

	uint8_t tipusCub = LLUM;

	
private:
	// Posa en CubActual el vector del cub al que estem mirant
	void ObtenirCubMira(); 
	glm::vec3 ObtenirCostat();


	void canviarModeMouse(int mode);

	void loop();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool _VSync = false;
	bool _Culling = true;
	
	GLFWwindow* window;
	glm::vec3 CubActual;
	glm::vec3 Costat;

	Camera camera;
	Renderer renderer;
	SuperChunk* mon;

	
};

