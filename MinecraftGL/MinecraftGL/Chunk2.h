#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <iostream>
#include<vector>

using namespace std;

#define X 16
#define Y 127 // 102 se empieza a ver raruno
#define Z 16

class Chunk2
{
public:
	Chunk2();
	~Chunk2();

	void canviarCub(int x, int y, int z, uint8_t tipus); // Canvia el tipus d'un cub concret
	uint8_t obtenirCub(int x, int y, int z); // Retorna el tipus del cub

	void update();
	void render();

	int nCubs() const;

private:
	static void afegirVertex(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z);
	void afegirCub(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z);

	unsigned int VBO;

	uint8_t chunk[X][Y][Z]; // Es guarda el tipus de cada cub

	unsigned int elements = 0;

	bool canviat = true; // Ens diu si ha canviat o no el chunk
};

