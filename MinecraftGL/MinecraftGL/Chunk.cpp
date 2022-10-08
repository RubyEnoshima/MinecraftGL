#include "Chunk.h"

Chunk::Chunk()
{
	glGenBuffers(1, &VBO);
	memset(chunk,0,X*Y*Z); // Posem tots a 0 (aire)
}

void Chunk::canviarCub(int x, int y, int z, uint8_t tipus)
{
	chunk[x][y][z] = tipus;
	canviat = true;
}

uint8_t Chunk::obtenirCub(int x, int y, int z)
{
	return chunk[x][y][z];
}

void Chunk::update()
{
	// Mostra un sol quadrat
	// després sera tvec4<GLbyte>
	float vertices[] = {
		// Primer triangle
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		// Segon triangle
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, 0.5f, 0.0f
	}; // TEMPORAL!!!!

	elements = 1;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	canviat = false;
}

void Chunk::render()
{
	if (canviat) update();

	glEnable(GL_CULL_FACE);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // EL SEGUNDO HAY QUE CAMBIARLO POR 4 Y GL_FLOAT POR GL_BYTE!!! USAREMOS BYTES POR EL TIPO
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glDisableVertexAttribArray(0);
}
