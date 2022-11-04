#pragma once
#include "Chunk2.h"
#include "Renderer.h"

#define XC 1
#define YC 1

class SuperChunk
{
public:
	SuperChunk(Renderer* _renderer);

	void canviarCub(int x, int y, int z, uint8_t tipus); // Canvia el tipus d'un cub concret
	uint8_t obtenirCub(int x, int y, int z); // Retorna el tipus del cub

	void update();
	void render();
private:
	Chunk2* Chunks[XC][YC];
	Renderer* renderer;
};

