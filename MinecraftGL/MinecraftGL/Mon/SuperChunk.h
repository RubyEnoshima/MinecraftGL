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

	// Canvia el tipus d'un cub concret
	void canviarCub(int x, int y, int z, uint8_t tipus); 
	// Canvia quanta llum té un cub
	void canviarLlumCub(int x, int y, int z, uint8_t llum); 
	// Retorna el tipus del cub
	uint8_t obtenirCub(int x, int y, int z); 
	// Retorna la llum del cub
	uint8_t obtenirLlumCub(int x, int y, int z);

	uint8_t obtenirLlumMaxima(const vector<glm::vec3>& cubs);

	void calcularLlum();

	// Genera un BoundingBox per un cub concret
	void BoundingBox(int x, int y, int z);

	void update();
	void render();
	// Renderitza un cub en una posició de manera que cada cara es pugui identificar pel color
	void renderCub(int x, int y, int z);
private:
	vector<glm::vec3> obtenirColindants(glm::vec3& pos) const;

	Chunk2* Chunks[XC][YC];
	Renderer* renderer;
	
	vector<glm::vec3> llums;


	//int octaves = 8;
	//float frequencia = 8.0;
	//float amplitud = 1.0;
	//int seed = 94;
};

