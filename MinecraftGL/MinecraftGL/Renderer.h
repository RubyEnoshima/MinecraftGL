#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

class Renderer
{
public:
	// Finestra
	int crearFinestra(); // Inicia OpenGL i crea una finestra. Retorna 1 si tot ha anat bé.
	void centrarFinestra(); // Centra la finestra
	GLFWwindow* finestra(); // Retorna la finestra principal
	pair<int, int> obtenirTamany() const;

	// Shaders
	int carregaShaders(); // Carrega els shaders. Retorna 1 si tot ha anat bé.
	unsigned int obtenirUniform(const char* uniform) const;
	void colocarMat4(const string uniform,const glm::mat4 matriu);

	// Monitor
	float aspectRatio() const;
private:
	// Arxius pels shaders
	const string vertexShaderSource = "VertexShader.vert";
	const string fragmentShaderSource = "FragmentShader.frag";

	unsigned int VAO, shaderProgram;

	GLFWwindow* window;

	const int width = 800;
	const int height = 600;
};

