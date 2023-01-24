#include "Chunk2.h"

Chunk2::Chunk2(unsigned int _x, unsigned int _y)
{
	memset(chunk, 6, sizeof(chunk));
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
	chunk[x][y][z] = tipus;
	canviat = true;

	if (x == 0 && veiEsq) veiEsq->canviat = true;
	else if (x == X - 1 && veiDre) veiDre->canviat = true;
	else if (z == 0 && veiBaix) veiBaix->canviat = true;
	else if (z == Z - 1 && veiUp) veiUp->canviat = true;
}

uint8_t Chunk2::obtenirCub(int x, int y, int z)
{
	return chunk[x][y][z];
}

void Chunk2::afegirVertex(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, bool u, bool v, bool r, bool g, bool b) {
	// Posicio
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	// Color
	vertices.push_back(r);
	vertices.push_back(g);
	vertices.push_back(b);
	// Coordenades de textura
	vertices.push_back(u);
	vertices.push_back(v);
	// Tipus (posició del mapa de textures)
	vertices.push_back(tipus%16);
	vertices.push_back(tipus/16);
}


void Chunk2::afegirCub(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus) {
	// Cara esq
	if ((x == 0 and veiEsq and !veiEsq->obtenirCub(X - 1, y, z) or (x == 0 and !veiEsq)) or (x != 0 and !chunk[x - 1][y][z])) {
		afegirVertex(vertices, x, y, z, tipus, 0, 1);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 1, 0);

	}

	// Cara dre
	if ((x == X - 1 and veiDre and !veiDre->obtenirCub(0, y, z) or (x == X - 1 and !veiDre)) or (x != X - 1 and !chunk[x + 1][y][z])) {
		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 0, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1);
	}

	// Cara frontal
	if ((z == Z - 1 and veiUp and !veiUp->obtenirCub(x, y, 0) or (z == Z - 1 and !veiUp)) or (z != Z - 1 and !chunk[x][y][z + 1])) {
		afegirVertex(vertices, x, y, z + 1, tipus, 0, 1);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0);

		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 0);

	}

	// Cara darrera
	if ((z == 0 and veiBaix and !veiBaix->obtenirCub(x, y, Z - 1) or (z == 0 and !veiBaix)) or (z != 0 and !chunk[x][y][z - 1])) {
		afegirVertex(vertices, x, y, z, tipus, 1, 1);
		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1);

		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 0, 0);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1);

	}

	// Cara adalt
	if (y == Y - 1 or !chunk[x][y + 1][z]) {
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 1);

	}

	// Cara sota
	if (y == 0 or !chunk[x][y - 1][z]) {
		afegirVertex(vertices, x, y, z, tipus, 0, 0);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0);

	}

}

void Chunk2::afegirCubFlat(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus) {
	// Cara esq
	if ((x == 0 and veiEsq and !veiEsq->obtenirCub(X - 1, y, z) or (x == 0 and !veiEsq)) or (x != 0 and !chunk[x - 1][y][z])) {
		afegirVertex(vertices, x, y, z, tipus, 0, 1, 0);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, 0);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, 0);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, 0);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, 0);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 1, 0, 0);

	}

	// Cara dre
	if ((x == X - 1 and veiDre and !veiDre->obtenirCub(0, y, z) or (x == X - 1 and !veiDre)) or (x != X - 1 and !chunk[x + 1][y][z])) {
		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, 0, 0);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, 0, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, 0, 0);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, 0, 0);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 0, 0, 0, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, 0, 0);
	}

	// Cara frontal
	if ((z == Z - 1 and veiUp and !veiUp->obtenirCub(x, y, 0) or (z == Z - 1 and !veiUp)) or (z != Z - 1 and !chunk[x][y][z + 1])) {
		afegirVertex(vertices, x, y, z + 1, tipus, 0, 1, 1, 0, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, 1, 0, 0);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, 1, 0, 0);

		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, 1, 0, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, 1, 0, 0);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 0, 1, 0, 0);

	}

	// Cara darrera
	if ((z == 0 and veiBaix and !veiBaix->obtenirCub(x, y, Z - 1) or (z == 0 and !veiBaix)) or (z != 0 and !chunk[x][y][z - 1])) {
		afegirVertex(vertices, x, y, z, tipus, 1, 1, 1, 0, 1);
		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, 1, 0, 1);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, 1, 0, 1);

		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, 1, 0, 1);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 0, 0, 1, 0, 1);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, 1, 0, 1);

	}

	// Cara adalt
	if (y == Y - 1 or !chunk[x][y + 1][z]) {
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, 0, 1, 0);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1, 0, 1, 0);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, 0, 1, 0);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, 0, 1, 0);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1, 0, 1, 0);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 1, 0, 1, 0);

	}

	// Cara sota
	if (y == 0 or !chunk[x][y - 1][z]) {
		afegirVertex(vertices, x, y, z, tipus, 0, 0, 1, 1, 0);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, 1, 1, 0);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0, 1, 1, 0);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, 1, 1, 0);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, 1, 1, 0);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0, 1, 1, 0);

	}

}


void Chunk2::update()
{
	vector<GLbyte> _vertices;

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				uint8_t tipus = chunk[i][j][k];
				if (tipus) afegirCub(_vertices, i, j, k, tipus);
			}
		}
	}

	elements = _vertices.size();
	vertices = _vertices;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, elements, vertices.data(), GL_STATIC_DRAW);

	canviat = false;
}

void Chunk2::render()
{
	if (canviat) update();


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)(3 * sizeof(GLbyte)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)(6 * sizeof(GLbyte)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_BYTE, GL_FALSE, 10 * sizeof(GLbyte), (void*)(8 * sizeof(GLbyte)));
	glEnableVertexAttribArray(3);*/

	
	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)(3 * sizeof(GLbyte)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)(6*sizeof(GLbyte)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)(2 * sizeof(bool) + 6 * sizeof(GLbyte)));
	glEnableVertexAttribArray(3);
	

	glDrawArrays(GL_TRIANGLES, 0, elements);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Chunk2::renderCub(int x, int y, int z)
{
	vector<GLbyte> _vertices;
	unsigned int VBO_FLAT;

	afegirCubFlat(_vertices, x, y, z, 14);
	glGenBuffers(1, &VBO_FLAT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_FLAT);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_FLAT);

	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 2 * sizeof(bool) + 8 * sizeof(GLbyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 2 * sizeof(bool) + 8 * sizeof(GLbyte), (void*)(3 * sizeof(GLbyte)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_BYTE, GL_FALSE, 2 * sizeof(bool) + 8 * sizeof(GLbyte), (void*)(6 * sizeof(GLbyte)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_BYTE, GL_FALSE, 2 * sizeof(bool) + 8 * sizeof(GLbyte), (void*)(2 * sizeof(bool) + 6 * sizeof(GLbyte)));
	glEnableVertexAttribArray(3);

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glDeleteBuffers(1, &VBO_FLAT);

	//canviat = true;
}

int Chunk2::nCubs() const
{
	return elements / 6;
}

void Chunk2::emplenarChunk()
{
	for (int i = 0; i < X; i++) {
		for (int j = Y / 2; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				canviarCub(i, j, k, 0);
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
