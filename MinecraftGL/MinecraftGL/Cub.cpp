#include "Cub.h"

Cub::Cub() {
	Cub(0, 0, 0, 0);
}

Cub::~Cub()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Cub::Cub(int x, int y, int z, int _id)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	for (int i = 0; i < 24; i+=3) {
		vertices[i] += x;
		vertices[i+1] += y;
		vertices[i+2] += z;
	}

	id = _id;
}

void Cub::update()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void Cub::render()
{
	update();

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // EL SEGUNDO HAY QUE CAMBIARLO POR 4 Y GL_FLOAT POR GL_BYTE!!! USAREMOS BYTES POR EL TIPO
	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
}

void Cub::canviaTipus(uint8_t _tipus)
{
	tipus = _tipus;
}
