#pragma once

#include "Shaders/ShaderProgram.h"
#include "Framebuffer.h"
#include "Textura.h"
#include "Recursos.h"

#define MAX_SHADERS 2

class Renderer
{
public:
	~Renderer();
	// Finestra
	int crearFinestra(); // Inicia OpenGL i crea una finestra. Retorna 1 si tot ha anat bé.
	void centrarFinestra(); // Centra la finestra
	GLFWwindow* finestra(); // Retorna la finestra principal
	pair<int, int> obtenirTamany() const; // Retorna el tamany de la finestra
	pair<int, int> obtenirCentre() const; // Retorna el centre de la finestra

	// Shaders
	int carregaShaders();
	void usarShader(int i = 0);
	ShaderProgram* obtShader();

	unsigned int obtenirUniform(const char* uniform) const;

	void colocarMat4(const string uniform, const glm::mat4 matriu);
	void colocarInt(const string uniform,int n);

	void activaAigua(bool aigua);
	void activaBounding(bool bounding);
	void activaNit(bool nit);

	// Llum (deprecated?)
	void canviarColorLlum(const glm::vec3 color);
	void canviarPosLlum(const glm::vec3 pos);
	
	float rgb(int color) const;

	// Framebuffer
	void DibuixarDarrera() const;
	void DibuixarFront() const;

	// Monitor
	float aspectRatio() const;

	void usarTexturaMon();
private:
	Textura* texturaMon;
	unsigned int shaderActual;

	ShaderProgram shaders[MAX_SHADERS];

	Framebuffer framebuffer;

	GLFWwindow* window;

	const int WIDTH = 1920;
	const int HEIGHT = 1080;
};

