#pragma once
#include "Chunk2.h"
#include "Renderer.h"

#define XC 4
#define YC 4

class SuperChunk
{
public:
	SuperChunk();
	~SuperChunk();
	SuperChunk(Renderer* _renderer);

	void canviarCub(int x, int y, int z, uint8_t tipus); // Canvia el tipus d'un cub concret
	uint8_t obtenirCub(int x, int y, int z); // Retorna el tipus del cub

	void BoundingBox(int x, int y, int z);

	void update();
	void render();
	void renderCub(int x, int y, int z);
private:
	Chunk2* Chunks[XC][YC];
	Renderer* renderer;
};

