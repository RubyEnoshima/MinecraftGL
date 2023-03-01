#pragma once
#include "Chunk2.h"
#include "Renderer/Renderer.h"
#include <queue>

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
	// Canvia quanta llum t� un cub
	void canviarLlumNaturalCub(int x, int y, int z, uint8_t llum);
	void canviarLlumArtificialCub(int x, int y, int z, uint8_t llum);
	// Retorna el tipus del cub
	uint8_t obtenirCub(int x, int y, int z); 
	// Retorna la llum del cub
	uint8_t obtenirLlumNaturalCub(int x, int y, int z);
	uint8_t obtenirLlumArtificialCub(int x, int y, int z);

	void afegirLlum(const glm::vec3 posLlum);
	void treureLlum(const glm::vec3 posLlum, uint8_t llumIni);

	// Genera un BoundingBox per un cub concret
	void BoundingBox(int x, int y, int z);

	void update();
	void render();
	// Renderitza un cub en una posici� de manera que cada cara es pugui identificar pel color
	void renderCub(int x, int y, int z);
private:
	void posarLlum(glm::vec3 pos, uint8_t llum);
	void eliminarLlum(glm::vec3 pos, uint8_t llum);

	Chunk2* Chunks[XC][YC];
	Renderer* renderer;
	
	// Propagaci� de llums "artificials"
	queue<glm::vec3> llums;
	queue<glm::vec3> cuaLlum;
	queue<pair<glm::vec3, uint8_t>> cuaLlumTreure;

	// Llum natural
	queue<glm::vec3> cuaLlumNatural;


	//int octaves = 8;
	//float frequencia = 8.0;
	//float amplitud = 1.0;
	//int seed = 94;
};

