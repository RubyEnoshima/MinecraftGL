#pragma once

#include "Mon/SuperChunk.h"
#include "Renderer/Renderer.h"
#include "Jugador/Jugador.h"
#include "HUD.h"
#include "Mon/Nuvols.h"

#include "glm/gtx/io.hpp"

#include <string>
#include <thread>
#include <mutex>

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

	void moure();

	uint8_t tipusCub = TERRA;
	HUD* _HUD;

	float deltaTime = 0.0f;
	Jugador* jugador = NULL;

	// Es guarda si una tecla s'esta prement en el moment
	map<int, bool> tecles = { {GLFW_KEY_W, false},{GLFW_KEY_A, false},{GLFW_KEY_S, false},{GLFW_KEY_D, false},{GLFW_KEY_SPACE, false},{GLFW_KEY_LEFT_SHIFT, false} };

private:
	// Posa en CubActual el vector del cub al que estem mirant
	void ObtenirCubMira(); 
	glm::vec3 ObtenirCostat();

	void canviarModeMouse(int mode);

	void loop();

	float lastFrame = 0.0f;

	bool _VSync = false;
	bool _Culling = true;
	
	GLFWwindow* window;
	glm::vec3 CubActual;
	glm::vec3 Costat;

	//Camera camera;
	Renderer renderer;
	SuperChunk* mon;
	
};

