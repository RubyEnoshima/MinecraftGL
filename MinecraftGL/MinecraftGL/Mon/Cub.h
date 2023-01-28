#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
class Cub
{
public:
	Cub();
	~Cub();
	Cub(int x, int y, int z, int _id);

	void update();
	void render();

	void canviaTipus(uint8_t _tipus);

private:
	unsigned int VBO, EBO;

	uint8_t tipus = 0;
	unsigned short id = 0;

	float vertices[8*3] = {
		0.5f, 0.5f, -0.5f, // arriba der del
		0.5f, -0.5f, -0.5f, // abajo der del
		-0.5f, -0.5f, -0.5f, // abajo izq del
		-0.5f, 0.5f, -0.5f, // arriba izq del

		-0.5f, 0.5f, 0.5f, // arriba izq det
		-0.5f, -0.5f, 0.5f, // abajo izq det
		0.5f, -0.5f, 0.5f, // abajo der det
		0.5f, 0.5f, 0.5f // arriba der det
	}; // TEMPORAL!!!!

	unsigned int indices[6*6] = {
		// Cara frontal
		0, 1, 3,
		2, 3, 1,

		// Cara der
		7, 6, 0,
		1, 0, 6,

		// Cara trasera
		4, 5, 7,
		6, 7, 5,

		// Cara izq
		3, 2, 4,
		5, 4, 2,

		// Cara arriba
		7, 0, 4,
		3, 4, 0,

		// Cara abajo
		1, 6, 2,
		5, 2, 6
	};
};

