#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

class Renderer
{
public:
	int crearFinestra(); // Inicia OpenGL i crea una finestra. Retorna 1 si tot ha anat bé.
	void centrarFinestra(); // Centra la finestra
	GLFWwindow* finestra(); // Retorna la finestra principal


	int carregaShaders(); // Carrega els shaders. Retorna 1 si tot ha anat bé.


private:
	// Arxius pels shaders
	const string vertexShaderSource = "VertexShader.vert";
	const string fragmentShaderSource = "FragmentShader.frag";

	unsigned int VAO, shaderProgram;

	GLFWwindow* window;

	const int width = 800;
	const int height = 600;
};

