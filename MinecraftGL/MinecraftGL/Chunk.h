#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cub.h"

#include <string.h>
#include <iostream>

using namespace std;

#define X 16
#define Y 1 // No se puede subir de 16 ?
#define Z 16

class Chunk
{
public:
	Chunk();

	void canviarCub(int x, int y, int z, uint8_t tipus); // Canvia el tipus d'un cub concret
	uint8_t obtenirCub(int x, int y, int z); // Retorna el tipus del cub

	void update();
	void render();

	int nCubs() const;

private:
	Cub chunk[X][Y][Z]; // Es guarda el cub
	//uint8_t chunk[X][Y][Z]; // Es guarda el tipus de cada cub

	unsigned int elements = 0;

	bool canviat = true; // Ens diu si ha canviat o no el chunk
};

