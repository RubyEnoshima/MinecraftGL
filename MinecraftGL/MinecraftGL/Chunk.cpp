#include "Chunk.h"

Chunk::Chunk()
{
	/*glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);*/
	//memset(chunk,0,X*Y*Z); // Posem tots a 0 (aire)

	int l = 0;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				chunk[i][j][k] = Cub(i,j,k,l);
				l++;
			}
		}
	}
}

//void Chunk::canviarCub(int x, int y, int z, uint8_t tipus)
//{
//	chunk[x][y][z] = tipus;
//	canviat = true;
//}

//uint8_t Chunk::obtenirCub(int x, int y, int z)
//{
//	return chunk[x][y][z];
//}

void Chunk::update()
{
	// Mostra un sol quadrat
	// després sera tvec4<GLbyte>
	
	//elements = 1;

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				chunk[i][j][k].update();
				elements++;
			}
		}
	}

	canviat = false;
}

void Chunk::render()
{
	if (canviat) update();

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				chunk[i][j][k].render();
			}
		}
	}
}

int Chunk::nCubs() const
{
	return elements;
}
