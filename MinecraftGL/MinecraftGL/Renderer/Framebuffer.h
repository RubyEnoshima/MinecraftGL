#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();
	Framebuffer(int width, int height);

	// Mètodes per emprar i deixar d'utilitzar el framebuffer
	void Unir() const;
	void Desunir() const;

	unsigned int ObtID() const;
	unsigned int ObtTextura() const;

private:
	unsigned int fbo;
	unsigned int textura;
};

