#include "Chunk.h"

Chunk::Chunk(unsigned int _x, unsigned int _y)
{
	memset(chunk, 0, sizeof(chunk));
	/*for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				chunk[i][j][k] = new Cub;
			}
		}
	}*/
	posX = _x;
	posY = _y;
}

Chunk::~Chunk()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_TRANSP);
	if (!Recursos::jocAcabat) {
		if (veiUp)veiUp->veiBaix = NULL;
		if (veiBaix)veiBaix->veiUp = NULL;
		if (veiEsq)veiEsq->veiDre = NULL;
		if (veiDre)veiDre->veiEsq = NULL;
	}
	_vertices.clear();
	/*for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				delete chunk[i][j][k];
			}
		}
	}*/
}

void Chunk::canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar, int color)
{
	if (esValid(x,y,z) || (!reemplacar && obtenirCub(x, y, z) != AIRE)) return;

	// Si volem posar vegetacio, ens asegurem que nomes es pot posar on volem
	if (Recursos::getBloc(tipus)->vegetacio) {
		uint8_t tipusBlocDebaix = obtenirCub(x, y - 1, z);
		if(tipusBlocDebaix != GESPA && tipusBlocDebaix != TERRA) return;
	}
	// Si volem treure el bloc i a sobre tenim vegetacio, tamb� treiem la vegetacio
	if (tipus == AIRE && Recursos::getBloc(obtenirCub(x, y + 1, z))->vegetacio)
		canviarCub(x, y + 1, z, AIRE);

	chunk[x][y][z].tipus = tipus;
	chunk[x][y][z].color = color;

	canviat = true;

	if (x == 0 && veiEsq) veiEsq->canviat = true;
	else if (x == X - 1 && veiDre) veiDre->canviat = true;
	if (z == 0 && veiBaix) veiBaix->canviat = true;
	else if (z == Z - 1 && veiUp) veiUp->canviat = true;
}

uint8_t Chunk::obtenirCub(int x, int y, int z) const
{
	if (esValid(x,y,z)) return 0;

	return chunk[x][y][z].tipus;
}

void Chunk::canviarLlumNaturalCub(int x, int y, int z, uint8_t llum)
{
	if (esValid(x,y,z)) return;

	chunk[x][y][z].llum = (chunk[x][y][z].llum & 0xF) | (llum << 4);
	canviat = true;
	generat = false;
}

void Chunk::canviarLlumArtificialCub(int x, int y, int z, uint8_t llum)
{
	if (esValid(x,y,z)) return;

	chunk[x][y][z].llum = (chunk[x][y][z].llum & 0xF0) | llum;
	canviat = true;
	generat = false;

}

uint8_t Chunk::obtenirLlumNaturalCub(int x, int y, int z) const
{
	if (esValid(x,y,z)) return 0;

	return (chunk[x][y][z].llum >> 4) & 0xF; // Retornem els primers 4 bits
}

uint8_t Chunk::obtenirLlumArtificialCub(int x, int y, int z) const
{
	if (esValid(x,y,z)) return 0;

	return chunk[x][y][z].llum & 0xF; // Retornem els ultims 4
}

void Chunk::afegirVertex(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, bool u, bool v, uint8_t llum, uint8_t costat, int color) {
	// Posicio
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	
	// Quantitat de llum
	vertices.push_back(llum);

	// Coordenades de textura
	vertices.push_back(u);
	vertices.push_back(v);

	// Tipus (posici� del mapa de textures)
	vertices.push_back(tipus%16);
	vertices.push_back(tipus/16);

	// Costat del cub que representa
	vertices.push_back(costat);

	// Color
	glm::vec3* _color = Recursos::obtColor(color);
	vertices.push_back(_color->r);
	vertices.push_back(_color->g);
	vertices.push_back(_color->b);
}

void Chunk::afegirCub(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus,int _color) {
	Bloc* b = Recursos::getBloc(tipus);
	uint8_t llum = chunk[x][y][z].llum;
	int color = _color;
	// Si es vegetaci�, la renderitzem amb 4 plans que sempre miren a la mateixa orientaci�
	if (b->vegetacio) {
		if (tipus == ARBUST) color = Recursos::VERDFULLES;
		// Pla 1
		afegirVertex(vertices, x, y, z, tipus, 0, 1, llum, 0, color);
		afegirVertex(vertices, x+1, y, z + 1, tipus, 1, 1, llum, 0, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x+1, y, z + 1, tipus, 1, 1, llum, 0, color);
		afegirVertex(vertices, x+1, y + 1, z + 1, tipus, 1, 0, llum, 0, color);

		afegirVertex(vertices, x+1, y, z+1, tipus, 0, 1, llum, 0, color); // 1
		afegirVertex(vertices, x, y, z, tipus, 1, 1, llum, 0, color); // 2
		afegirVertex(vertices, x+1, y + 1, z+1, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x+1, y + 1, z+1, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x, y, z, tipus, 1, 1, llum, 0, color); // 5
		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum, 0, color);
		// Pla 2
		afegirVertex(vertices, x+1, y, z, tipus, 0, 1, llum, 0, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum, 0, color);
		afegirVertex(vertices, x+1, y + 1, z, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x+1, y + 1, z, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum, 0, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 1, 0, llum, 0, color);

		afegirVertex(vertices, x, y, z + 1, tipus, 0, 1, llum, 0, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, llum, 0, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, llum, 0, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum, 0, color);
		return;
	}

	bool transp = b->id == FULLES;
	tipus = b->costats;
	// Cara esq
	if ((transp) or (x == 0 and veiEsq != NULL and (!veiEsq->obtenirCub(X - 1, y, z) || (!b->transparent && Recursos::getBloc(veiEsq->obtenirCub(X - 1, y, z))->transparent))) or (x != 0 and (!chunk[x - 1][y][z].tipus or (!b->transparent && Recursos::getBloc(chunk[x - 1][y][z].tipus)->transparent)))) {
		if (x == 0) { if (veiEsq) llum = veiEsq->chunk[X - 1][y][z].llum; }
		else llum = chunk[x - 1][y][z].llum;

		afegirVertex(vertices, x, y, z, tipus, 0, 1, llum, 1, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum, 1, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum, 1, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum, 1, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum, 1, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 1, 0, llum, 1, color);

	}
	// Cara dre
	if ((transp) or (x == X - 1 and veiDre != NULL and (!veiDre->obtenirCub(0, y, z) || (!b->transparent && Recursos::getBloc(veiDre->obtenirCub(0, y, z))->transparent))) or (x != X - 1 and (!chunk[x + 1][y][z].tipus or (!b->transparent && Recursos::getBloc(chunk[x + 1][y][z].tipus)->transparent)))) {
		if (x == X - 1) { if (veiDre) llum = veiDre->chunk[0][y][z].llum; }
		else llum = chunk[x + 1][y][z].llum;

		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, llum, 1, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum, 1, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, llum, 1, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum, 1, color);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 0, 0, llum, 1, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, llum, 1, color);

	}

	// Cara frontal
	if ((transp) or (z == Z - 1 and veiUp != NULL and (!veiUp->obtenirCub(x, y, 0) || (!b->transparent && Recursos::getBloc(veiUp->obtenirCub(x, y, 0))->transparent))) or (z != Z - 1 and (!chunk[x][y][z + 1].tipus or (!b->transparent && Recursos::getBloc(chunk[x][y][z + 1].tipus)->transparent)))) {
		if (z == Z - 1) { if (veiUp) llum = veiUp->chunk[x][y][0].llum; }
		else llum = chunk[x][y][z + 1].llum;

		afegirVertex(vertices, x, y, z + 1, tipus, 0, 1, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum, 2, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum, 2, color);

		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum, 2, color);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 0, llum, 2, color);

	}

	// Cara darrera
	if ((transp) or (z == 0 and veiBaix != NULL and (!veiBaix->obtenirCub(x, y, Z - 1) || (!b->transparent && Recursos::getBloc(veiBaix->obtenirCub(x, y, Z - 1))->transparent))) or (z != 0 and (!chunk[x][y][z - 1].tipus or (!b->transparent && Recursos::getBloc(chunk[x][y][z - 1].tipus)->transparent)))) {
		if (z == 0) { if (veiBaix) llum = veiBaix->chunk[x][y][Z - 1].llum; }
		else llum = chunk[x][y][z - 1].llum;

		afegirVertex(vertices, x, y, z, tipus, 1, 1, llum, 2, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum, 2, color);

		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 0, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum, 2, color);

	}

	tipus = b->adalt;
	// Cara adalt
	if ((transp) or y == Y - 1 or !chunk[x][y + 1][z].tipus or (!b->transparent && Recursos::getBloc(chunk[x][y + 1][z].tipus)->transparent) or (tipus == AIGUA && chunk[x][y + 1][z].tipus == CRISTAL)) {
		if (y == Y - 1) llum = 0;
		else llum = chunk[x][y + 1][z].llum;

		if (tipus == GESPA) {
			color = Recursos::VERDGESPA;
		}

		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum, 0, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1, llum, 0, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum, 0, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum, 0, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 1, llum, 0, color);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 1, llum, 0, color);
		color = _color;
	}

	tipus = b->sota;
	// Cara sota
	if ((transp) or y == 0 or !chunk[x][y - 1][z].tipus or (!b->transparent && Recursos::getBloc(chunk[x][y - 1][z].tipus)->transparent)) {
		if (y == Y - 1) llum = 0;
		else if(y>0) llum = chunk[x][y - 1][z].llum;

		afegirVertex(vertices, x, y, z, tipus, 0, 0, llum, 3, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum, 3, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0, llum, 3, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum, 3, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum, 3, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 0, llum, 3, color);

	}
	
}

void Chunk::afegirVertexFlat(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, bool r, bool g, bool b) {
	// Posicio
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	// Color
	vertices.push_back(r);
	vertices.push_back(g);
	vertices.push_back(b);
}

void Chunk::afegirCubFlat(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus) {
	// Cara esq
	if ((x == 0 and veiEsq and !veiEsq->obtenirCub(X - 1, y, z) or veiEsq->obtenirCub(X - 1, y, z)==AIGUA or (x == 0 and !veiEsq)) or (x != 0 and !chunk[x - 1][y][z].tipus)) {
		afegirVertexFlat(vertices, x, y, z, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z, 0);
		afegirVertexFlat(vertices, x, y + 1, z, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 0);

	}

	// Cara dre
	if ((x == X - 1 and veiDre and !veiDre->obtenirCub(0, y, z) or veiDre->obtenirCub(0, y, z)==AIGUA or (x == X - 1 and !veiDre)) or (x != X - 1 and !chunk[x + 1][y][z].tipus)) {
		afegirVertexFlat(vertices, x + 1, y, z, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z + 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 0, 0);
	}

	// Cara frontal
	if ((z == Z - 1 and veiUp and !veiUp->obtenirCub(x, y, 0) or veiUp->obtenirCub(x, y, 0)==AIGUA or (z == Z - 1 and !veiUp)) or (z != Z - 1 and !chunk[x][y][z + 1].tipus)) {
		afegirVertexFlat(vertices, x, y, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 1, 0, 0);

		afegirVertexFlat(vertices, x, y + 1, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 1, 0, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z + 1, 1, 0, 0);

	}

	// Cara darrera
	if ((z == 0 and veiBaix and !veiBaix->obtenirCub(x, y, Z - 1) or veiBaix->obtenirCub(x, y, Z - 1)==AIGUA or (z == 0 and !veiBaix)) or (z != 0 and !chunk[x][y][z - 1].tipus)) {
		afegirVertexFlat(vertices, x, y, z, 1, 0, 1);
		afegirVertexFlat(vertices, x, y + 1, z, 1, 0, 1);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 0, 1);

		afegirVertexFlat(vertices, x, y + 1, z, 1, 0, 1);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 1, 0, 1);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 0, 1);

	}

	// Cara adalt
	if (y == Y - 1 or !chunk[x][y + 1][z].tipus or chunk[x][y + 1][z].tipus==AIGUA) {
		afegirVertexFlat(vertices, x, y + 1, z, 0, 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 0, 1, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 1, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z, 0, 1, 0);
		afegirVertexFlat(vertices, x, y + 1, z + 1, 0, 1, 0);
		afegirVertexFlat(vertices, x + 1, y + 1, z + 1, 0, 1, 0);

	}

	// Cara sota
	if (y == 0 or !chunk[x][y - 1][z].tipus or chunk[x][y - 1][z].tipus==AIGUA) {
		afegirVertexFlat(vertices, x, y, z, 1, 1, 0);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 1, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 1, 1, 0);
		afegirVertexFlat(vertices, x + 1, y, z, 1, 1, 0);
		afegirVertexFlat(vertices, x + 1, y, z + 1, 1, 1, 0);
		afegirVertexFlat(vertices, x, y, z + 1, 1, 1, 0);

	}

}

bool Chunk::esValid(int x, int y, int z) const
{
	return x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z;
}

bool Chunk::renderCub(int x, int y, int z)
{
	vector<GLubyte> _vertices;
	unsigned int VBO_FLAT;

	afegirCubFlat(_vertices, x, y, z, 0);
	if (_vertices.empty()) return false;
	glGenBuffers(1, &VBO_FLAT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_FLAT);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_FLAT);

	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 6 * sizeof(GLubyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 6 * sizeof(GLubyte), (void*)(3 * sizeof(GLubyte)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glDeleteBuffers(1, &VBO_FLAT);
	return true;
}

void Chunk::crearVertexs()
{
	if (!canviat || generat || descarregant) return;
	vector<GLubyte> vertices, vertices_transp;

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				uint8_t tipus = chunk[i][j][k].tipus;
				if (tipus) {
					if(Recursos::getBloc(tipus)->semitransparent) afegirCub(vertices_transp, i, j, k, tipus, chunk[i][j][k].color);
					else afegirCub(vertices, i, j, k, tipus, chunk[i][j][k].color);
				}
			}
		}
	}

	elements = vertices.size();
	_vertices = vertices;
	elements_transp = vertices_transp.size();
	_vertices_transp = vertices_transp;
	generat = true;
	if (!carregat) {
		if (veiEsq) { veiEsq->canviat = true; }
		if (veiDre) { veiDre->canviat = true; }
		if (veiUp) { veiUp->canviat = true; }
		if (veiBaix) { veiBaix->canviat = true; }
		carregat = true;
	}

}

void Chunk::update()
{
	if (VBO == 0) { glGenBuffers(1, &VBO); glGenBuffers(1, &VBO_TRANSP); }
	if (!generat || elements == 0) return;
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TRANSP);
	glBufferData(GL_ARRAY_BUFFER, elements_transp, _vertices_transp.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, elements, _vertices.data(), GL_STATIC_DRAW);

	canviat = false;
	generat = false;
}

void Chunk::render(bool semi)
{
	if (!preparat || !carregat) return;
	if (canviat) update();

	unsigned int vbo_bind = VBO, ele = elements;
	if (semi) { vbo_bind = VBO_TRANSP; ele = elements_transp; }

	glBindBuffer(GL_ARRAY_BUFFER, vbo_bind);

	glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, 12 * sizeof(GLubyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, 12 * sizeof(GLubyte), (void*)(3 * sizeof(GLubyte)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_UNSIGNED_BYTE, GL_FALSE, 12 * sizeof(GLubyte), (void*)(4 * sizeof(GLubyte)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_UNSIGNED_BYTE, GL_FALSE, 12 * sizeof(GLubyte), (void*)(6 * sizeof(GLubyte)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_UNSIGNED_BYTE, GL_FALSE, 12 * sizeof(GLubyte), (void*)(8 * sizeof(GLubyte)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 3, GL_UNSIGNED_BYTE, GL_FALSE, 12 * sizeof(GLubyte), (void*)(9 * sizeof(GLubyte)));
	glEnableVertexAttribArray(5);
	

	glDrawArrays(GL_TRIANGLES, 0, ele);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}

bool Chunk::esVisible(Frustum* frustum) const
{
	glm::vec3 min = { posX * X, -Y/1.5, posY * Z }, max = {posX*X + X, Y*1.75, posY * Z + Z};
	glm::vec3 centre = (min + max) * 0.5f;
	glm::vec3 extents = (max - min) * 0.5f;

	for (const Pla& pla : frustum->obtPlans()) {

		float distancia = glm::dot(centre - pla.pos, pla.normal);

		if (distancia < -glm::length(extents) / 4) return false;
	}
	
	return true;
}

int Chunk::nCubs() const
{
	return elements / 6;
}

glm::vec2 Chunk::obtPos() const
{
	return glm::vec2(posX,posY);
}

vector<pair<int,glm::vec3>> Chunk::emplenarChunk(const Generador&generador)
{
	vector<pair<int, glm::vec3>> res;
	const int W = 0, H = 0;
	for (int i = 0; i < X; i++) {
		for (int k = 0; k < Z; k++) {

			int height = Y/2;
			float x = (W + i + X * posX);
			float y = (H + k + Z * posY);
			
			if (generador.tipusMon == NORMAL) {
				height = generador.obtAltura(x,y);
			}
			
			height += 5;

			for (int j = 0; j <= height; j++) {
				int tipus = generador.obtTipus(j, height);
				if (tipus == GESPA || tipus == NEU) {
					float probArbre = (float)(rand()) / (float)(RAND_MAX);
					float probFlor = (float)(rand()) / (float)(RAND_MAX);

					if (probArbre < generador.probabilitatArbre) {
						// Marquem l'arbre
						res.push_back({ 0, glm::vec3(i + X * posX,j + 1,k + Z * posY) });
					}
					else if (probFlor < generador.probabilitatFlor) {
						// Marquem la flor
						res.push_back({ 1, glm::vec3(i + X * posX,j + 1,k + Z * posY) });
					}

				}
				canviarCub(i, j, k, tipus, true, Recursos::BLANC);

			}
			for (int j = 0; j < generador.nivellMar; j++) {
				canviarCub(i, j, k, generador.aigua, false, Recursos::AIGUA);

			}
		}
	}

	return res;

}


void Chunk::afegirVeins(Chunk* left, Chunk* right, Chunk* up, Chunk* down)
{
	veiEsq = left;
	veiDre = right;
	veiUp = up;
	veiBaix = down;
	if (left) { left->veiDre = this; /*left->canviat = true;*/ }
	if (right) { right->veiEsq = this; /*right->canviat = true;*/}
	if (up) { up->veiBaix = this; /*up->canviat = true;*/ }
	if (down) { down->veiUp = this; /*down->canviat = true;*/ }
}
