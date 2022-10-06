#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "Triangle.h"

using namespace std;

class Joc
{
public:
	void canviaShaders();
	void prepararDibuix(Triangle t);
	void dibuixarObjecte(int modo, int n);

	void gameLoop();

	int crearFinestra();
	
private:
	void processInput();
	void loop();

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	unsigned int VAO, shaderProgram;
	bool mode = 0; // 0 normal, 1 wireframe
	GLFWwindow* window;
};

