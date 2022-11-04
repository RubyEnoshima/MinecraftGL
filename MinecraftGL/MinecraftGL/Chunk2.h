#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <iostream>
#include <vector>
#include "Tipus.h"

using namespace std;

#define X 16
#define Y 127 // 102 se empieza a ver raruno
#define Z 16

class Chunk2
{
public:
	Chunk2(unsigned int _x, unsigned int _y);
	~Chunk2();

	void canviarCub(int x, int y, int z, uint8_t tipus); // Canvia el tipus d'un cub concret
	uint8_t obtenirCub(int x, int y, int z); // Retorna el tipus del cub

	void update();
	void render();

	int nCubs() const;

	void emplenarChunk(); // Funció debug
	void afegirVeins(Chunk2* left=NULL, Chunk2* right= NULL, Chunk2* up = NULL, Chunk2* down = NULL);

private:
	static void afegirVertex(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z);
	void afegirCub(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z);

	unsigned int VBO;

	unsigned int posX, posY;
	uint8_t chunk[X][Y][Z]; // Es guarda el tipus de cada cub

	unsigned int elements = 0;

	bool canviat = true; // Ens diu si ha canviat o no el chunk

	Chunk2* veiEsq = NULL;
	Chunk2* veiDre = NULL;
	Chunk2* veiUp = NULL;
	Chunk2* veiBaix = NULL;
};

