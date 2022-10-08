#pragma once

#include "Chunk.h"
#include "Renderer.h"
#include "Camera.h"

using namespace std;

static class Joc
{
public:
	void gameLoop(); // Fa que comenci el joc i executa el loop

	int crearFinestra(); // Retorna 1 si s'ha pogut crear la finestra
	
private:
	void canviarModeMouse(int mode);

	void loop();

	Renderer renderer;
	GLFWwindow* window;
	

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	
};

