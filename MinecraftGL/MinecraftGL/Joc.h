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
	
	Camera camera;
	Renderer renderer;
	SuperChunk* mon;
	
private:
	void canviarModeMouse(int mode);

	void loop();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	
	GLFWwindow* window;
};

