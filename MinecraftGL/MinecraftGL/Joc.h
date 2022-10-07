#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>

#include "Chunk.h"

using namespace std;

class Joc
{
public:
	void gameLoop();

	int crearFinestra();
	
private:
	void processInput();

	int carregaShaders();

	void loop();

	const string vertexShaderSource = "VertexShader.vert";

	const string fragmentShaderSource = "FragmentShader.frag"; 

	unsigned int VAO, shaderProgram;
	bool mode = 0; // 0 normal, 1 wireframe
	GLFWwindow* window;

	const int width = 800;
	const int height = 600;
};

