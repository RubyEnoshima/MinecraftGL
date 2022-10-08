#include "Chunk.h"

Chunk::Chunk()
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
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
		0.5f, 0.5f, -0.5f, // arriba der del
		0.5f, -0.5f, -0.5f, // abajo der del
		-0.5f, -0.5f, -0.5f, // abajo izq del
		-0.5f, 0.5f, -0.5f, // arriba izq del

		-0.5f, 0.5f, 0.5f, // arriba izq det
		-0.5f, -0.5f, 0.5f, // abajo izq det
		0.5f, -0.5f, 0.5f, // abajo der det
		0.5f, 0.5f, 0.5f // arriba der det
	}; // TEMPORAL!!!!

	unsigned int indices[] = { // note that we start from 0!
		// Cara frontal
		0, 1, 3,
		2, 3, 1, 

		// Cara der
		7, 6, 0,
		1, 0, 6,

		// Cara trasera
		4, 5, 7,
		6, 7, 5,

		// Cara izq
		3, 2, 4,
		5, 4, 2,

		// Cara arriba
		7, 0, 4,
		3, 4, 0,

		// Cara abajo
		1, 6, 2,
		5, 2, 6
	};


	elements = 1;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	canviat = false;
}

void Chunk::render()
{
	if (canviat) update();

	glEnable(GL_CULL_FACE); // victima o verdugo

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // EL SEGUNDO HAY QUE CAMBIARLO POR 4 Y GL_FLOAT POR GL_BYTE!!! USAREMOS BYTES POR EL TIPO
	glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);
}
