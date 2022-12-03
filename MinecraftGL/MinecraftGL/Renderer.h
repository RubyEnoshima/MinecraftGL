#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "Textura.h"

class Renderer
{
public:
	// Finestra
	int crearFinestra(); // Inicia OpenGL i crea una finestra. Retorna 1 si tot ha anat bé.
	void centrarFinestra(); // Centra la finestra
	GLFWwindow* finestra(); // Retorna la finestra principal
	pair<int, int> obtenirTamany() const; // Retorna el tamany de la finestra
	pair<int, int> obtenirCentre() const; // Retorna el centre de la finestra

	// Shaders
	int carregaShaders();
	void useShader();
	unsigned int obtenirUniform(const char* uniform) const;
	void colocarMat4(const string uniform,const glm::mat4 matriu);
	void canviarColor(const glm::vec4 color);

	float rgb(int color) const;

	// Monitor
	float aspectRatio() const;
private:

	unsigned int VAO, shaderActual;

	ShaderProgram shader;

	GLFWwindow* window;

	const int width = 800;
	const int height = 600;
};

