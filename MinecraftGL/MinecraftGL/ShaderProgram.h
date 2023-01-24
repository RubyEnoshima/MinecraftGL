#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

private:
	// Arxius pels shaders
	string vertexShaderSource = "VertexShader.vert";
	string fragmentShaderSource = "FragmentShader.frag";
	unsigned int shaderProgram;
};

