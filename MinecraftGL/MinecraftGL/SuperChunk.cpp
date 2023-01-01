#include "SuperChunk.h"

SuperChunk::SuperChunk()
{
	renderer = nullptr;
	for (int x = 0; x < XC; x++)
		for (int y = 0; y < YC; y++)
			Chunks[x][y] = nullptr;
}

SuperChunk::~SuperChunk() {
	for (int x = 0; x < XC; x++)
		for (int y = 0; y < YC; y++)
				delete Chunks[x][y];
}

SuperChunk::SuperChunk(Renderer* _renderer)
{
	renderer = _renderer;

	for (int i = 0; i < XC; i++)
	{
		for (int j = 0; j < YC; j++)
		{
			Chunks[i][j] = new Chunk2(i,j);
		}
	}

	for (int i = 0; i < XC; i++)
	{
		for (int j = 0; j < YC; j++)
		{
			Chunk2* up = NULL;
			Chunk2* left = NULL;
			Chunk2* right = NULL;
			Chunk2* down = NULL;
			if (i - 1 >= 0) left = Chunks[i - 1][j];
			if (i + 1 < XC) right = Chunks[i + 1][j];
			if (j - 1 >= 0) down = Chunks[i][j - 1];
			if (j + 1 < YC) up = Chunks[i][j + 1];
			Chunks[i][j]->afegirVeins(left, right, up, down);
			Chunks[i][j]->emplenarChunk();
		}
	}

}

void SuperChunk::canviarCub(int x, int y, int z, uint8_t tipus)
{
	Chunks[x / X][y / Y][z / Z].canviarCub(x%X,y,z%Z,tipus);
}

uint8_t SuperChunk::obtenirCub(int x, int y, int z)
{
	return Chunks[(x / X)][y/Y][(z / Z)].obtenirCub(x % X, y, z % Z);
}

void SuperChunk::BoundingBox(int x, int y, int z)
{
	if (x != X && y != Y && z != Z)
		Chunks[x / X][y / Y][z / Z].BoundingBox(x % X, y, z % Z);
}

void SuperChunk::update()
{
	
}

void SuperChunk::render()
{	
	if (renderer) {
		for (int i = 0; i < XC; i++)
		{
			for (int j = 0; j < YC; j++)
			{
				if (Chunks[i][j]) {
					// Hem de moure el chunk per tal que no estiguin tots al mateix lloc
					glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(i * X - X * XC / 2, -Y/2, j * Z - Z * YC / 2));
					renderer->colocarMat4("model", model);
					Chunks[i][j]->render();
				}

			}
		}
	}
	
}

void SuperChunk::renderCub(int x, int y, int z)
{
	//glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x - X * XC / 2, -Y / 2, z - Z * YC / 2));
	//renderer->colocarMat4("model", model);
	Chunks[(x / X)][y / Y][(z / Z)].renderCub(x % X, y, z % Z);
}
