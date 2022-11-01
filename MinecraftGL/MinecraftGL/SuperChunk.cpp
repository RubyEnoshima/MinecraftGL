#include "SuperChunk.h"

SuperChunk::SuperChunk(Renderer* _renderer)
{
	renderer = _renderer;
	for (int i = 0; i < XC; i++)
	{
		for (int j = 0; j < YC; j++)
		{
			Chunks[i][j] = new Chunk2();
		}
	}
}

void SuperChunk::update()
{
	
}

void SuperChunk::render()
{	
	for (int i = 0; i < XC; i++)
	{
		for (int j = 0; j < YC; j++)
		{
			if (Chunks[i][j]) {
				// Hem de moure el chunk per tal que no estiguin tots al mateix lloc
				glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(i * X - X*XC/2, -Y, j * Z - Z * YC / 2));
				renderer->colocarMat4("model",model);
				Chunks[i][j]->render();
			}
			
		}
	}
}
