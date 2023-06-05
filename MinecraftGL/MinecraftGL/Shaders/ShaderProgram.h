#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

class ShaderProgram
{
public:
	ShaderProgram(string vertex = "VertexShader.vert",string frag = "FragmentShader.frag");

	int carregaShaders(); // Carrega els shaders. Retorna 1 si tot ha anat bé.
	void usar();
	int getProgram() const;

	unsigned int obtenirUniform(const char* uniform) const;
	void colocarMat4(const string uniform, const glm::mat4 matriu);
	void colocarVec3(const string uniform, const glm::vec3 vector);
	void colocarInt(const string uniform, int i);
private:
	// Arxius pels shaders
	string vertexShaderSource = "VertexShader.vert";
	string fragmentShaderSource = "FragmentShader.frag";
	unsigned int shaderProgram;
};

