#include "Chunk.h"

Chunk::Chunk(unsigned int _x, unsigned int _y, Blocs* _blocs)
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
	//mon = _mon;
	blocs = _blocs;
}

Chunk::~Chunk()
{
	glDeleteBuffers(1, &VBO);
	if (veiUp)veiUp->veiBaix = NULL;
	if (veiBaix)veiBaix->veiUp = NULL;
	if (veiEsq)veiEsq->veiDre = NULL;
	if (veiDre)veiDre->veiEsq = NULL;
	/*for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				delete chunk[i][j][k];
			}
		}
	}*/
}

void Chunk::canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar, char* color)
{
	if ((x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) || (!reemplacar && obtenirCub(x, y, z) != AIRE)) return;

	// Si volem posar vegetacio, ens asegurem que nomes es pot posar on volem
	if (blocs->getBloc(tipus)->vegetacio) {
		uint8_t tipusBlocDebaix = obtenirCub(x, y - 1, z);
		if(tipusBlocDebaix != GESPA && tipusBlocDebaix != TERRA) return;
	}
	// Si volem treure el bloc i a sobre tenim vegetacio, també treiem la vegetacio
	if (tipus == AIRE && blocs->getBloc(obtenirCub(x, y + 1, z))->vegetacio)
		canviarCub(x, y + 1, z, AIRE);

	chunk[x][y][z].tipus = tipus;
	chunk[x][y][z].color = color;

	canviat = true;
	unCanviat = true;
	cubCanviat = glm::vec3(x,y,z);

	if (x == 0 && veiEsq) veiEsq->canviat = true;
	else if (x == X - 1 && veiDre) veiDre->canviat = true;
	if (z == 0 && veiBaix) veiBaix->canviat = true;
	else if (z == Z - 1 && veiUp) veiUp->canviat = true;
}

uint8_t Chunk::obtenirCub(int x, int y, int z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return 0;

	return chunk[x][y][z].tipus;
}

void Chunk::canviarLlumNaturalCub(int x, int y, int z, uint8_t llum)
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return;

	chunk[x][y][z].llum = (chunk[x][y][z].llum & 0xF) | (llum << 4);
	canviat = true;

}

void Chunk::canviarLlumArtificialCub(int x, int y, int z, uint8_t llum)
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return;

	chunk[x][y][z].llum = (chunk[x][y][z].llum & 0xF0) | llum;
	canviat = true;

}

uint8_t Chunk::obtenirLlumNaturalCub(int x, int y, int z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return 0;

	return (chunk[x][y][z].llum >> 4) & 0xF; // Retornem els primers 4 bits
}

uint8_t Chunk::obtenirLlumArtificialCub(int x, int y, int z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return 0;

	return chunk[x][y][z].llum & 0xF; // Retornem els ultims 4
}

void Chunk::afegirVertex(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, bool u, bool v, uint8_t llum, uint8_t costat, const char* color) {
	// Posicio
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	
	// Quantitat de llum
	vertices.push_back(llum);

	// Coordenades de textura
	vertices.push_back(u);
	vertices.push_back(v);

	// Tipus (posició del mapa de textures)
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

void Chunk::afegirCub(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus,const char* _color) {
	Bloc* b = blocs->getBloc(tipus);
	uint8_t llum = chunk[x][y][z].llum;
	const char* color = _color;
	// Si es vegetació, la renderitzem amb 4 plans que sempre miren a la mateixa orientació
	if (b->vegetacio) {
		// Pla 1
		afegirVertex(vertices, x, y, z, tipus, 0, 1, llum);
		afegirVertex(vertices, x+1, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x+1, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x+1, y + 1, z + 1, tipus, 1, 0, llum);

		afegirVertex(vertices, x+1, y, z+1, tipus, 0, 1, llum); // 1
		afegirVertex(vertices, x, y, z, tipus, 1, 1, llum); // 2
		afegirVertex(vertices, x+1, y + 1, z+1, tipus, 0, 0, llum);
		afegirVertex(vertices, x+1, y + 1, z+1, tipus, 0, 0, llum);
		afegirVertex(vertices, x, y, z, tipus, 1, 1, llum); // 5
		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum);
		// Pla 2
		afegirVertex(vertices, x+1, y, z, tipus, 0, 1, llum);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x+1, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x+1, y + 1, z, tipus, 0, 0, llum);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 1, 0, llum);

		afegirVertex(vertices, x, y, z + 1, tipus, 0, 1, llum);
		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, llum);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum);
		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, llum);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum);
		return;
	}
	if (x == 0 && z == 14 && y == 70 && posX == 0 && posY==-2) {
		cout << "chicri";
	}
	tipus = b->costats;
	// Cara esq
	if ((x == 0 and veiEsq != NULL and (!veiEsq->obtenirCub(X - 1, y, z) || blocs->getBloc(veiEsq->obtenirCub(X - 1, y, z))->transparent)) or (x != 0 and (!chunk[x - 1][y][z].tipus or blocs->getBloc(chunk[x - 1][y][z].tipus)->transparent))) {
		if (x == 0) llum = veiEsq->chunk[X - 1][y][z].llum;
		else llum = chunk[x - 1][y][z].llum;

		afegirVertex(vertices, x, y, z, tipus, 0, 1, llum, 1, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum, 1, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum, 1, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 0, 0, llum, 1, color);
		afegirVertex(vertices, x, y, z + 1, tipus, 1, 1, llum, 1, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 1, 0, llum, 1, color);

		if (carregat && veiCanviat == 0) {
			veiCanviat = -1;
			return;
		}
	}
	// Cara dre
	if ((x == X - 1 and veiDre != NULL and (!veiDre->obtenirCub(0, y, z) || blocs->getBloc(veiDre->obtenirCub(0, y, z))->transparent)) or (x != X - 1 and (!chunk[x + 1][y][z].tipus or blocs->getBloc(chunk[x + 1][y][z].tipus)->transparent))) {
		if (x == X - 1) llum = veiDre->chunk[0][y][z].llum;
		else llum = chunk[x + 1][y][z].llum;

		afegirVertex(vertices, x + 1, y, z, tipus, 1, 1, llum, 1, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum, 1, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, llum, 1, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 1, 0, llum, 1, color);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 0, 0, llum, 1, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 0, 1, llum, 1, color);
		
		if (carregat && veiCanviat == 1) {
			veiCanviat = -1;
			return;
		}
	}

	// Cara frontal
	if ((z == Z - 1 and veiUp != NULL and (!veiUp->obtenirCub(x, y, 0) || blocs->getBloc(veiUp->obtenirCub(x, y, 0))->transparent)) or (z != Z - 1 and (!chunk[x][y][z + 1].tipus or blocs->getBloc(chunk[x][y][z + 1].tipus)->transparent))) {
		if (z == Z - 1) llum = veiUp->chunk[x][y][0].llum;
		else llum = chunk[x][y][z + 1].llum;
		
		afegirVertex(vertices, x, y, z + 1, tipus, 0, 1, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum, 2, color);
		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum, 2, color);

		afegirVertex(vertices, x, y + 1, z + 1, tipus, 0, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z + 1, tipus, 1, 1, llum, 2, color);
		afegirVertex(vertices, x + 1, y + 1, z + 1, tipus, 1, 0, llum, 2, color);

		if (carregat && veiCanviat == 2) {
			veiCanviat = -1;
			return;
		}
	}

	// Cara darrera
	if ((z == 0 and veiBaix != NULL and (!veiBaix->obtenirCub(x, y, Z - 1) || blocs->getBloc(veiBaix->obtenirCub(x, y, Z - 1))->transparent)) or (z != 0 and (!chunk[x][y][z - 1].tipus or blocs->getBloc(chunk[x][y][z - 1].tipus)->transparent))) {
		if (z == 0) llum = veiBaix->chunk[x][y][Z - 1].llum;
		else llum = chunk[x][y][z - 1].llum;
		
		afegirVertex(vertices, x, y, z, tipus, 1, 1, llum, 2, color);
		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum, 2, color);

		afegirVertex(vertices, x, y + 1, z, tipus, 1, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y + 1, z, tipus, 0, 0, llum, 2, color);
		afegirVertex(vertices, x + 1, y, z, tipus, 0, 1, llum, 2, color);

		if (carregat && veiCanviat == 3) {
			veiCanviat = -1;
			return;
		}
	}

	tipus = b->adalt;
	// Cara adalt
	if (y == Y - 1 or !chunk[x][y + 1][z].tipus or blocs->getBloc(chunk[x][y + 1][z].tipus)->transparent) {
		if (y == Y - 1) llum = 0;
		else llum = chunk[x][y + 1][z].llum;

		if (tipus == GESPA) {
			color = "VerdGespa";
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
	if (y == 0 or !chunk[x][y - 1][z].tipus or blocs->getBloc(chunk[x][y - 1][z].tipus)->transparent) {
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
	vector<GLubyte> vertices;

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				uint8_t tipus = chunk[i][j][k].tipus;
				if (tipus) {
					//if (tipus == LLUM) llum = 12;
					afegirCub(vertices, i, j, k, tipus, chunk[i][j][k].color);
				}
			}
		}
	}

	elements = vertices.size();
	_vertices = vertices;
	generat = true;
	if (!carregat) {
		if (veiEsq) { veiEsq->canviat = true; }
		if (veiDre) { veiDre->canviat = true; }
		if (veiUp) { veiUp->canviat = true; }
		if (veiBaix) { veiBaix->canviat = true; }
		carregat = true;
	}

	/*if (veiEsq) {
		veiEsq->veiCanviat = 1; veiEsq->canviat = true;
	}
	if (veiDre) { veiDre->veiCanviat = 0; veiDre->canviat = true;}
	if (veiUp) { veiUp->veiCanviat = 3; veiUp->canviat = true; }
	if (veiBaix) { veiBaix->veiCanviat = 2; veiBaix->canviat = true; }*/
}

void Chunk::update()
{
	if(VBO == 0) glGenBuffers(1, &VBO);
	if (!generat || elements == 0) return;
	//crearVertexs();
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, elements, _vertices.data(), GL_STATIC_DRAW);

	canviat = false;
	generat = false;
}

void Chunk::render()
{
	if (!preparat || !carregat) return;
	if (canviat) update();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
	

	glDrawArrays(GL_TRIANGLES, 0, elements);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}

int Chunk::nCubs() const
{
	return elements / 6;
}

glm::vec2 Chunk::obtPos() const
{
	return glm::vec2(posX,posY);
}

vector<pair<int,glm::vec3>> Chunk::emplenarChunk(int tipus)
{
	vector<pair<int, glm::vec3>> res;
	const int W = 500, H = 500;
	for (int i = 0; i < X; i++) {
		for (int k = 0; k < Z; k++) {

			int height = Y / 2;
			if(tipus == Recursos::NORMAL) height = Y/2 + (int)(glm::perlin(glm::vec2((float)(W + i + X * posX) / X, (float)(H + k + Z * posY) / Z)) * 15);

			for (int j = 0; j <= height; j++) {
				uint8_t tipus = TERRA;
				if (j == height) { // Capa d'adalt
					tipus = GESPA;
					//color = glm::vec3(0.8f, 0.8f, 0.5f);
					float probArbre = (float)(rand()) / (float)(RAND_MAX);
					float probFlor = (float)(rand()) / (float)(RAND_MAX);
					
					if (probArbre < probabilitatArbre) {
						// Marquem l'arbre
						res.push_back({0, glm::vec3(i + X * posX,j + 1,k + Z * posY) });
					}
					else if (probFlor < probabilitatFlor) {
						// Marquem la flor
						res.push_back({ 1, glm::vec3(i + X * posX,j + 1,k + Z * posY) });
					}
				}
				else if (j < height - 3) tipus = PEDRA;
				else if (j < 2) tipus = BEDROCK;
				canviarCub(i, j, k, tipus, true, (char *)"Blanc");

			}
		}
	}

	unCanviat = false;

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

bool Chunk::cubTop(int8_t x, int8_t y, int8_t z) const
{
	if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) return false;
	if (y == Y - 1) return true;
	//return chunk[x][y][z].top;

	for (int i = y + 1; i < Y; i++) {
		if (obtenirCub(x, i, z) != 0) return false;
	}

	return true;
}
