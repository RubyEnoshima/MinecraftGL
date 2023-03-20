#include "Chunk2.h"

Chunk2::Chunk2(unsigned int _x, unsigned int _y)
{
	memset(chunk, 0, sizeof(chunk));
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
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return;

	chunk[x][y][z].tipus = tipus;

	canviat = true;

	if (x == 0 && veiEsq) veiEsq->canviat = true;
	else if (x == X - 1 && veiDre) veiDre->canviat = true;
	if (z == 0 && veiBaix) veiBaix->canviat = true;
	else if (z == Z - 1 && veiUp) veiUp->canviat = true;
}

uint8_t Chunk2::obtenirCub(int x, int y, int z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return 0;

	return chunk[x][y][z].tipus;
}

void Chunk2::canviarLlumNaturalCub(int x, int y, int z, uint8_t llum)
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return;

	chunk[x][y][z].llum = (chunk[x][y][z].llum & 0xF) | (llum << 4);
	canviat = true;

}

void Chunk2::canviarLlumArtificialCub(int x, int y, int z, uint8_t llum)
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return;

	chunk[x][y][z].llum = (chunk[x][y][z].llum & 0xF0) | llum;
	canviat = true;

}

uint8_t Chunk2::obtenirLlumNaturalCub(int x, int y, int z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return 0;

	return (chunk[x][y][z].llum >> 4) & 0xF; // Retornem els primers 4 bits
}

uint8_t Chunk2::obtenirLlumArtificialCub(int x, int y, int z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return 0;

	return chunk[x][y][z].llum & 0xF; // Retornem els ultims 4
}

uint8_t Chunk2::obtenirLlumNaturalMaxima(int x, int y, int z) const
{
	// MIRAR SI FUNCIONA BÉ
	uint8_t res = 0;
	uint8_t llum;
	// Esq
	if (x - 1 < 0 && veiEsq && veiEsq->obtenirCub(X - 1, y, z) == AIRE) {
		llum = veiEsq->obtenirLlumNaturalCub(X - 1, y, z);
		if (llum > res) res = llum;
	}
	else if (x - 1 >= 0) {
		llum = obtenirLlumNaturalCub(x - 1, y, z);
		if (llum > res) res = llum;
	}
	// Dre
	if (x + 1 >= X && veiDre && veiDre->obtenirCub(0, y, z) == AIRE) {
		llum = veiDre->obtenirLlumNaturalCub(0, y, z);
		if (llum > res) res = llum;
	}
	else if (x + 1 < X) {
		llum = obtenirLlumNaturalCub(x + 1, y, z);
		if (llum > res) res = llum;
	}
	// Davant
	if (z + 1 >= Z && veiBaix && veiBaix->obtenirCub(x, y, 0) == AIRE) {
		llum = veiBaix->obtenirLlumNaturalCub(x, y, 0);
		if (llum > res) res = llum;
	}
	else if (z + 1 < Z) {
		llum = obtenirLlumNaturalCub(x, y, z + 1);
		if (llum > res) res = llum;
	}
	// Darrera 
	if (z - 1 < 0 && veiUp && veiUp->obtenirCub(x, y, Z - 1) == AIRE) {
		llum = veiUp->obtenirLlumNaturalCub(x, y, Z - 1);
		if (llum > res) res = llum;
	}
	else if (z - 1 >= 0) {
		obtenirLlumNaturalCub(x, y, z - 1);
		if (llum > res) res = llum;
	}
	return res;
}

void Chunk2::afegirVertex(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, bool u, bool v, uint8_t llum) {
	// Posicio
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);

	// Color
	//vertices.push_back(r);
	//vertices.push_back(g);
	//vertices.push_back(b);
	
	// Quantitat de llum
	vertices.push_back(llum);

	// Coordenades de textura
	vertices.push_back(u);
	vertices.push_back(v);

	// Tipus (posició del mapa de textures)
	vertices.push_back(tipus%16);
	vertices.push_back(tipus/16);

}


void Chunk2::afegirCub(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus) {
	uint8_t llum;
	if (tipus == GESPA) tipus = GESPA_COSTAT;
	else if (tipus == NEU) tipus = NEU_COSTAT;
	// Cara esq
	if ((x == 0 and veiEsq and !veiEsq->obtenirCub(X - 1, y, z)) or (x != 0 and (!chunk[x - 1][y][z].tipus or chunk[x-1][y][z].tipus==CRISTAL))) {
		if (x == 0) llum = veiEsq->chunk[X - 1][y][z].llum;
		else llum = chunk[x - 1][y][z].llum;

		afegirVertex(vertices, x, y, z, tipus, 0, 1, llum);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 1, 0, llum);

	}

	// Cara dre
	if ((x == X - 1 and veiDre and !veiDre->obtenirCub(0, y, z)) or (x != X - 1 and (!chunk[x + 1][y][z].tipus or chunk[x + 1][y][z].tipus == CRISTAL))) {
		llum = chunk[x + 1][y][z].llum;
		if (x == X - 1) llum = veiDre->chunk[0][y][z].llum;

		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, llum);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, llum);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 0, 0, llum);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, llum);
	}

	// Cara frontal
	if ((z == Z - 1 and veiUp and !veiUp->obtenirCub(x, y, 0)) or (z != Z - 1 and (!chunk[x][y][z + 1].tipus or chunk[x][y][z + 1].tipus ==CRISTAL))) {
		llum = chunk[x][y][z + 1].llum;
		if (z == Z - 1) llum = veiUp->chunk[x][y][0].llum;
		
		afegirVertex(vertices, x, y, z + 1, tipus, 0, 1, llum);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum);

		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 0, llum);

	}

	// Cara darrera
	if ((z == 0 and veiBaix and !veiBaix->obtenirCub(x, y, Z - 1)) or (z != 0 and (!chunk[x][y][z - 1].tipus or chunk[x][y][z - 1].tipus ==CRISTAL))) {
		llum = chunk[x][y][z - 1].llum;
		if (z == 0) llum = veiBaix->chunk[x][y][Z - 1].llum;
		
		afegirVertex(vertices, x, y, z, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum);

		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum);

	}

	if (tipus == GESPA_COSTAT) tipus = GESPA;
	else if (tipus == NEU_COSTAT) tipus = NEU;
	// Cara adalt
	if (y == Y - 1 or !chunk[x][y + 1][z].tipus or chunk[x][y + 1][z].tipus == CRISTAL) {
		llum = chunk[x][y + 1][z].llum;
		if (y == Y - 1) llum = 0;

		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1, llum);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1, llum);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 1, llum);

	}


	if (tipus == GESPA || tipus == NEU) tipus = TERRA;
	// Cara sota
	if (y == 0 or !chunk[x][y - 1][z].tipus or chunk[x][y - 1][z].tipus == CRISTAL) {
		llum = chunk[x][y - 1][z].llum;
		if (y == Y - 1) llum = 0;

		afegirVertex(vertices, x, y, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0, llum);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0, llum);

	}

}

void Chunk2::afegirVertexFlat(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, bool r, bool g, bool b) {
	// Posicio
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	// Color
	vertices.push_back(r);
	vertices.push_back(g);
	vertices.push_back(b);
}

void Chunk2::afegirCubFlat(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus) {
	// Cara esq
	if ((x == 0 and veiEsq and !veiEsq->obtenirCub(X - 1, y, z) or (x == 0 and !veiEsq)) or (x != 0 and !chunk[x - 1][y][z].tipus)) {
		afegirVertexFlat(vertices, x, y, z, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z, 0);
		afegirVertexFlat(vertices, x, y + 1, z, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 0);

	}

	// Cara dre
	if ((x == X - 1 and veiDre and !veiDre->obtenirCub(0, y, z) or (x == X - 1 and !veiDre)) or (x != X - 1 and !chunk[x + 1][y][z].tipus)) {
		afegirVertexFlat(vertices, x + 1, y, z, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z + 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 0, 0);
	}

	// Cara frontal
	if ((z == Z - 1 and veiUp and !veiUp->obtenirCub(x, y, 0) or (z == Z - 1 and !veiUp)) or (z != Z - 1 and !chunk[x][y][z + 1].tipus)) {
		afegirVertexFlat(vertices, x, y, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 1, 0, 0);

		afegirVertexFlat(vertices, x, y + 1, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z + 1, 1, 0, 0);

	}

	// Cara darrera
	if ((z == 0 and veiBaix and !veiBaix->obtenirCub(x, y, Z - 1) or (z == 0 and !veiBaix)) or (z != 0 and !chunk[x][y][z - 1].tipus)) {
		afegirVertexFlat(vertices, x, y, z, 1, 0, 1);
		afegirVertexFlat(vertices, x, y + 1, z, 1, 0, 1);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 0, 1);

		afegirVertexFlat(vertices, x, y + 1, z, 1, 0, 1);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 1, 0, 1);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 0, 1);

	}

	// Cara adalt
	if (y == Y - 1 or !chunk[x][y + 1][z].tipus) {
		afegirVertexFlat(vertices, x, y + 1, z, 0, 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 0, 1, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 1, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 0, 1, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z + 1, 0, 1, 0);

	}

	// Cara sota
	if (y == 0 or !chunk[x][y - 1][z].tipus) {
		afegirVertexFlat(vertices, x, y, z, 1, 1, 0);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 1, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 1, 1, 0);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 1, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 1, 1, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 1, 1, 0);

	}

}


void Chunk2::update()
{
	vector<GLbyte> _vertices;

	//if (unCanviat) {

	//	for (int i = -1; i <= 1; i++) {
	//		for (int j = -1; j <= 1; j++) {
	//			for (int k = -1; k <= 1; k++) {
	//				
	//				int posX = cubCanviat.x + i;
	//				int posY = cubCanviat.y + j;
	//				int posZ = cubCanviat.z + k;
	//				uint8_t tipus = chunk[posX][posY][posZ];
	//				//cout << i << endl;
	//				if (tipus) afegirCub(_vertices, posX, posY, posZ, tipus);
	//			}
	//		}
	//	}

	//}
	//else {
		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Y; j++) {
				for (int k = 0; k < Z; k++) {
					uint8_t tipus = chunk[i][j][k].tipus;
					if (tipus) {
						//if (tipus == LLUM) llum = 12;
						afegirCub(_vertices, i, j, k, tipus);
					} 
				}
			}
		}
	//}

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


	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 8 * sizeof(GLbyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_BYTE, GL_FALSE, 8 * sizeof(GLbyte), (void*)(3 * sizeof(GLbyte)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_BYTE, GL_FALSE, 8 * sizeof(GLbyte), (void*)(4 * sizeof(GLbyte)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_BYTE, GL_FALSE, 8 * sizeof(GLbyte), (void*)(6 * sizeof(GLbyte)));
	glEnableVertexAttribArray(3);

	/*
	// Posició
	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)0);
	glEnableVertexAttribArray(0);
	// Normals
	glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)(3 * sizeof(GLbyte)));
	glEnableVertexAttribArray(1);
	// UVs
	glVertexAttribPointer(2, 2, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)(6*sizeof(GLbyte)));
	glEnableVertexAttribArray(2);
	// Tipus (posició al mapa de textures)
	glVertexAttribPointer(3, 2, GL_BYTE, GL_FALSE, 2*sizeof(bool) + 8 * sizeof(GLbyte), (void*)(2 * sizeof(bool) + 6 * sizeof(GLbyte)));
	glEnableVertexAttribArray(3);
	*/
	

	glDrawArrays(GL_TRIANGLES, 0, elements);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Chunk2::renderCub(int x, int y, int z)
{
	vector<GLbyte> _vertices;
	//unsigned int VBO_FLAT;

	afegirCubFlat(_vertices, x, y, z, 0);
	//glGenBuffers(1, &VBO_FLAT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO_FLAT);

	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 6 * sizeof(GLbyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 6 * sizeof(GLbyte), (void*)(3 * sizeof(GLbyte)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, elements);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//glDeleteBuffers(1, &VBO_FLAT);
}

int Chunk2::nCubs() const
{
	return elements / 6;
}

void Chunk2::emplenarChunk(uint8_t llumNatural)
{
	for (int i = 0; i < X; i++) {
		for (int k = 0; k < Z; k++) {

			int height = Y/2 + (int)(glm::perlin(glm::vec2((float)(i + X * posX) / X, (float)(k + Z * posY) / Z)) * 8);

			for (int j = 0; j <= height; j++) {
				uint8_t tipus = TERRA;
				if (j == height) { // Capa d'adalt
					tipus = GESPA;

					chunk[i][j][k].top = true;
				}
				else if (j < height - 3) tipus = PEDRA;
				else if (j < 2) tipus = BEDROCK;
				canviarCub(i, j, k, tipus);
			}
		}
	}

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				if (cubTop(i,j,k))
				{
					canviarLlumNaturalCub(i, j+1, k, 15);
				}
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

bool Chunk2::cubTop(int8_t x, int8_t y, int8_t z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return false;
	if (y == Y - 1) return true;
	//return chunk[x][y][z].top;

	for (int i = y + 1; i < Y; i++) {
		if (obtenirCub(x, i, z) != 0) return false;
	}

	return true;
}
