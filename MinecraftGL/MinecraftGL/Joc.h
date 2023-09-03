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

const struct CompararVec3 {
	bool operator()(const glm::vec3& a, const glm::vec3& b) const {
		if (a.x < b.x) {
			return true;
		}
		if (a.x > b.x) {
			return false;
		}

		if (a.y < b.y) {
			return true;
		}
		if (a.y > b.y) {
			return false;
		}

		return a.z < b.z;
	}
};

class Joc
{
public:
	~Joc();

	// Fa que comenci el joc i executa el loop
	void start(); 

	// Retorna 1 si s'ha pogut crear la finestra
	int crearFinestra(); 

	void canviarProjeccio();

	// -- Funcions per quan es prem alguna tecla o botó --

	void DestruirCub();
	void PosarCub(uint8_t tipus);

	void Usar();
	
	void Culling();
	void VSync();
	void Frustum();
	void HUDDebug();
	void Inventari();
	void CanviaHora();

	void moure();
	void CanviarMode();

	HUD* _HUD;

	float deltaTime = 0.0f;
	Jugador* jugador = NULL;

	// Es guarda si una tecla s'esta prement en el moment
	map<int, bool> tecles = { {GLFW_KEY_W, false},{GLFW_KEY_A, false},{GLFW_KEY_S, false},{GLFW_KEY_D, false},{GLFW_KEY_SPACE, false},{GLFW_KEY_LEFT_SHIFT, false} };
	bool modeInventari = false;

	glm::vec2 obtMousePos() const;
private:
	void canviarModeMouse(int mode);

	// Posa en CubActual el vector del cub al que estem mirant
	void ObtenirCubMira(); 
	glm::vec3 ObtenirCostat();

	void loop();

	float lastFrame = 0.0f;

	bool _VSync = false;
	bool _Culling = true;
	int mode = ESPECTADOR;
	bool nit = false;

	const map<glm::vec3, glm::vec3, CompararVec3> colorsCostat = {
		{ {1,0,1},{0,0,-1}},
		{ {1,1,0},{0,-1,0}},
		{ {1,0,0},{0,0,1}},
		{ {0,1,1},{-1,0,0}},
		{ {0,1,0},{0,1,0}},
		{ {0,0,1},{1,0,0}}
	};
	
	GLFWwindow* window;
	glm::vec3 CubActual;
	glm::vec3 Costat;

	Renderer renderer;
	SuperChunk* mon;
	Nuvols* nuvols;
};

