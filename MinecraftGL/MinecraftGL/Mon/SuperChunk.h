#pragma once
#include "Chunk2.h"
#include "Renderer/Renderer.h"

#define XC 3
#define YC 3

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
	
	int octaves = 8;
	float frequencia = 8.0;
	float amplitud = 1.0;
	int seed = 94;
};

