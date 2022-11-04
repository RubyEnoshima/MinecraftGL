#include "Chunk2.h"

Chunk2::Chunk2(unsigned int _x, unsigned int _y)
{
	memset(chunk, TERRA, sizeof(chunk));
	glGenBuffers(1, &VBO);
	posX = _x;
	posY = _y;
}

Chunk2::~Chunk2()
{
	glDeleteBuffers(1, &VBO);

}

void Chunk2::canviarCub(int x, int y, int z, uint8_t tipus)
{
	cout << posX << posY << endl;
	chunk[x][y][z] = tipus;
	canviat = true;
}

uint8_t Chunk2::obtenirCub(int x, int y, int z)
{
	return chunk[x][y][z];
}

void Chunk2::afegirVertex(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z) {
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

void Chunk2::afegirCub(vector<GLbyte>&vertices, int8_t x, int8_t y, int8_t z) {
	// Cara esq
	if ((x==0 and veiEsq and !veiEsq->obtenirCub(X-1,y,z) or (x==0 and !veiEsq)) or (x!=0 and !chunk[x - 1][y][z])) {
		afegirVertex(vertices, x, y, z);
		afegirVertex(vertices, x, y, z+1);
		afegirVertex(vertices, x, y+1, z);
		afegirVertex(vertices, x, y+1, z);
		afegirVertex(vertices, x, y, z+1);
		afegirVertex(vertices, x, y+1, z+1);

	}

	// Cara dre
	if ((x == X - 1 and veiDre and !veiDre->obtenirCub(0,y,z) or (x == X-1 and !veiDre)) or (x != X - 1 and !chunk[x + 1][y][z])) {
		afegirVertex(vertices, x + 1, y, z);
		afegirVertex(vertices, x + 1, y + 1, z);
		afegirVertex(vertices, x + 1, y, z + 1);
		afegirVertex(vertices, x + 1, y + 1, z);
		afegirVertex(vertices, x + 1, y + 1, z + 1);
		afegirVertex(vertices, x + 1, y, z + 1);

	}

	// Cara frontal
	if ((z == Z - 1 and veiUp and !veiUp->obtenirCub(x,y,0) or (z == Z - 1 and !veiUp)) or (z != Z - 1 and !chunk[x][y][z + 1])) {
		afegirVertex(vertices, x, y, z + 1);
		afegirVertex(vertices, x + 1, y, z + 1);
		afegirVertex(vertices, x, y + 1, z + 1);
		afegirVertex(vertices, x, y + 1, z + 1);
		afegirVertex(vertices, x + 1, y, z + 1);
		afegirVertex(vertices, x + 1, y + 1, z + 1);

	}

	// Cara darrera
	if ((z == 0 and veiBaix and !veiBaix->obtenirCub(x,y,Z-1) or (z == 0 and !veiBaix)) or (z != 0 and !chunk[x][y][z - 1])) {
		afegirVertex(vertices, x, y, z);
		afegirVertex(vertices, x, y + 1, z);
		afegirVertex(vertices, x + 1, y, z);
		afegirVertex(vertices, x, y + 1, z);
		afegirVertex(vertices, x + 1, y + 1, z);
		afegirVertex(vertices, x + 1, y, z);

	}

	// Cara adalt
	if (y==Y-1 or !chunk[x][y + 1][z]) {
		afegirVertex(vertices, x, y + 1, z);
		afegirVertex(vertices, x, y + 1, z + 1);
		afegirVertex(vertices, x + 1, y + 1, z);
		afegirVertex(vertices, x + 1, y + 1, z);
		afegirVertex(vertices, x, y + 1, z + 1);
		afegirVertex(vertices, x + 1, y + 1, z + 1);

	}

	// Cara sota
	if (y == 0 or !chunk[x][y - 1][z]) {
		afegirVertex(vertices, x, y, z);
		afegirVertex(vertices, x + 1, y, z);
		afegirVertex(vertices, x, y, z + 1);
		afegirVertex(vertices, x + 1, y, z);
		afegirVertex(vertices, x + 1, y, z + 1);
		afegirVertex(vertices, x, y, z + 1);

	}

}

void Chunk2::update()
{
	vector<GLbyte> vertices;

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				uint8_t tipus = chunk[i][j][k];
				if(tipus) afegirCub(vertices,i,j,k);
			}
		}
	}

	elements = vertices.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, elements, vertices.data(), GL_STATIC_DRAW);
	
	canviat = false;
}

void Chunk2::render()
{
	if (canviat) update();

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 0, (void*)0); // EL SEGUNDO HAY QUE CAMBIARLO POR 4
	glDrawArrays(GL_TRIANGLES, 0, elements);

	glDisableVertexAttribArray(0);
}

int Chunk2::nCubs() const
{
	return elements/6;
}

void Chunk2::emplenarChunk()
{
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				canviarCub(i,j,k,1);
			}
		}
	}
}

void Chunk2::afegirVeins(Chunk2* left, Chunk2* right, Chunk2* up, Chunk2* down)
{
	veiEsq = left;
	veiDre = right;
	veiUp = up;
	veiBaix = down;

}
