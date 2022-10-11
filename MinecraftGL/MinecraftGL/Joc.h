#pragma once

#include "Chunk.h"
#include "Chunk2.h"
#include "Renderer.h"
#include "Camera.h"

using namespace std;

static class Joc
{
public:
	void gameLoop(); // Fa que comenci el joc i executa el loop

	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	int crearFinestra(); // Retorna 1 si s'ha pogut crear la finestra
	
private:
	void canviarModeMouse(int mode);

	void loop();

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	Renderer renderer;
	GLFWwindow* window;
	Camera camera;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	
};

