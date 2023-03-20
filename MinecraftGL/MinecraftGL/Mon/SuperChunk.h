#pragma once
#include "Chunk2.h"
#include "Renderer/Renderer.h"
#include <queue>
#include <list>

#define XC 12
#define YC 12

class SuperChunk
{
public:
	SuperChunk();
	~SuperChunk();
	SuperChunk(Renderer* _renderer);

	// Canvia el tipus d'un cub concret
	void canviarCub(int x, int y, int z, uint8_t tipus); 
	// Canvia quanta llum té un cub
	void canviarLlumNaturalCub(int x, int y, int z, uint8_t llum);
	void canviarLlumArtificialCub(int x, int y, int z, uint8_t llum);
	// Retorna el tipus del cub
	uint8_t obtenirCub(int x, int y, int z); 
	// Retorna la llum del cub
	uint8_t obtenirLlumNaturalCub(int x, int y, int z);
	uint8_t obtenirLlumArtificialCub(int x, int y, int z);

	void afegirLlum(const glm::vec3 posLlum, bool natural=false);
	void treureLlum(const glm::vec3 posLlum, uint8_t llumIni);

	// Genera un BoundingBox per un cub concret
	void BoundingBox(int x, int y, int z);

	void update();
	void render();
	// Renderitza un cub en una posició de manera que cada cara es pugui identificar pel color
	void renderCub(int x, int y, int z);
private:
	void posarLlum(glm::vec3 pos, uint8_t llum, bool natural=false, bool avall=false);
	void eliminarLlum(glm::vec3 pos, uint8_t llum);

	Chunk2* Chunks[XC][YC];
	Renderer* renderer;
	
	// Propagació de llums "artificials"
	queue<glm::vec3> llums;
	queue<glm::vec3> cuaLlum;
	queue<pair<glm::vec3, uint8_t>> cuaLlumTreure;
	list<glm::vec3> llistaLlums;

	// Llum natural
	queue<glm::vec3> cuaLlumNatural;

	uint8_t llumNatural = 15;
	//int octaves = 8;
	//float frequencia = 8.0;
	//float amplitud = 1.0;
	//int seed = 94;
};

