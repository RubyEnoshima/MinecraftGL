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
	glDeleteBuffers(1, &VAO);
}

SuperChunk::SuperChunk(Renderer* _renderer)
{
	srand(semilla);

	renderer = _renderer;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	for (int i = 0; i < XC; i++)
	{
		for (int j = 0; j < YC; j++)
		{
			Chunks[i][j] = new Chunk2(i,j,&blocs);
		}
	}
	vector<glm::vec3> arbrets;

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
			vector<pair<int, glm::vec3>> estructures = Chunks[i][j]->emplenarChunk();
			for (int i = 0; i < estructures.size(); i++) {
				int tipus = estructures[i].first;
				glm::vec3 pos = estructures[i].second;

				if (tipus == 0) arbrets.push_back(pos);
				else if (tipus == 1) {
					int tipusFlor = rand() % flors.size();
					canviarCub(pos.x, pos.y, pos.z, flors[tipusFlor], false);
				}
			}
			

			
		}
	}

	for (int i = 0; i < arbrets.size(); i++) {
		glm::vec3 pos = arbrets[i];
		arbre(pos.x,pos.y,pos.z);
	}

	for (int i = 0; i < X * XC; i++) {
		for (int k = 0; k < Z * YC; k++) {
			calculaLlumNatural(i,k);

		}
	}
}

void SuperChunk::calculaLlumNatural(int x, int z)
{
	int j = Y - 1;
	uint8_t llum = 15;
	while (blocs.getBloc(obtenirCub(x, j, z))->transparent) {
		uint8_t tipus = obtenirCub(x, j, z);
		if (tipus != AIRE && !blocs.getBloc(obtenirCub(x, j, z))->vegetacio) llum--;
		canviarLlumNaturalCub(x, j, z, llum);
		j--;
	}
	queue<glm::vec3> cua;
	glm::vec3 pos;
	Bloc* b;
	
	while (j >= 0) {
		uint8_t tipus = obtenirCub(x, j, z);
		b = blocs.getBloc(obtenirCub(x, j, z));
		pos = glm::vec3(x, j, z);
		if (b->transparent) cua.emplace(pos);
		j--;
	}
	set<string> visitades;
	while (!cua.empty()) {
		pos = cua.front();
		cua.pop();
		visitades.insert(glm::to_string(pos));
		uint8_t max = 1;
		for (int i = 0; i < 6; i++) {
			glm::vec3 actual = glm::vec3(pos.x + posicions[i].x, pos.y + posicions[i].y, pos.z + posicions[i].z);
			if (!blocs.getBloc(obtenirCub(actual.x, actual.y, actual.z))->transparent) continue;
			llum = obtenirLlumNaturalCub(actual.x,actual.y,actual.z);
			//cout << llum << endl;
			if (llum == 0 && visitades.find(glm::to_string(actual)) == visitades.end()) cua.emplace(actual);
			if (llum > max) {
				max = llum;
			}
				
			
		}
		canviarLlumNaturalCub(pos.x, pos.y, pos.z, max-1);
		//cout << max - 1 << endl;
	}
	//if(pos!=NULL) delete pos;
}

// LLUM ARTIFICIAL

void SuperChunk::posarLlum(glm::vec3 pos, uint8_t llum) {
	// Mirem si és transparent i si hem de pujar la llum
	if (blocs.getBloc(obtenirCub(pos.x, pos.y, pos.z))->transparent && obtenirLlumArtificialCub(pos.x, pos.y, pos.z) + 2 <= llum) {
		uint8_t resLlum = llum - 1;
		canviarLlumArtificialCub(pos.x, pos.y, pos.z, resLlum);
		cuaLlum.emplace(pos);
	}
}

void SuperChunk::eliminarLlum(glm::vec3 pos, uint8_t llum) {
	uint8_t llumCub = obtenirLlumArtificialCub(pos.x, pos.y, pos.z);
	if (llumCub != 0 && llumCub < llum) {
		canviarLlumArtificialCub(pos.x, pos.y, pos.z, 0);
		cuaLlumTreure.emplace(pair<glm::vec3, uint8_t>(pos,llumCub));
	}
	else if (llumCub >= llum) {
		llums.emplace(pos);
	}
}

void SuperChunk::afegirLlum(const glm::vec3 posLlum)
{
	// Fem una cua de posicions i afegim la llum
	cuaLlum.emplace(posLlum);

	while (!cuaLlum.empty()) {
		glm::vec3 bloc = cuaLlum.front();
		cuaLlum.pop();
		uint8_t llum = obtenirLlumArtificialCub(bloc.x, bloc.y, bloc.z);
		posarLlum(bloc + glm::vec3(1, 0, 0), llum);
		posarLlum(bloc + glm::vec3(-1, 0, 0), llum);
		posarLlum(bloc + glm::vec3(0, 1, 0), llum);
		posarLlum(bloc + glm::vec3(0, -1, 0), llum);
		posarLlum(bloc + glm::vec3(0, 0, 1), llum);
		posarLlum(bloc + glm::vec3(0, 0, -1), llum);

	}
}

void SuperChunk::treureLlum(const glm::vec3 posLlum, uint8_t llumIni)
{
	cuaLlumTreure.emplace(pair<glm::vec3, uint8_t>(posLlum,llumIni));
	canviarLlumArtificialCub(posLlum.x, posLlum.y, posLlum.z, 0);

	while (!cuaLlumTreure.empty()) {
		glm::vec3 bloc = cuaLlumTreure.front().first;
		uint8_t llum = cuaLlumTreure.front().second;
		cuaLlumTreure.pop();

		eliminarLlum(bloc + glm::vec3(1, 0, 0), llum);
		eliminarLlum(bloc + glm::vec3(-1, 0, 0), llum);
		eliminarLlum(bloc + glm::vec3(0, 1, 0), llum);
		eliminarLlum(bloc + glm::vec3(0, -1, 0), llum);
		eliminarLlum(bloc + glm::vec3(0, 0, 1), llum);
		eliminarLlum(bloc + glm::vec3(0, 0, -1), llum);

	}

	while (!llums.empty()) {
		afegirLlum(llums.front());
		llums.pop();
	}
}

// LLUM NATURAL

void SuperChunk::posarLlumNatural(glm::vec3 pos, uint8_t llum, bool avall) {
	// Mirem si és transparent (de moment 0, aire) i si hem de pujar la llum
	if (obtenirCub(pos.x, pos.y, pos.z) == AIRE && obtenirLlumNaturalCub(pos.x, pos.y, pos.z) + 2 <= llum) {
		uint8_t resLlum = llum - 1;
		if(avall && llum == llumNatural) resLlum = llum;
		canviarLlumNaturalCub(pos.x, pos.y, pos.z, resLlum);
		cuaLlumNatural.emplace(pos);
	}
}

void SuperChunk::afegirLlumNatural(const glm::vec3 posLlum)
{
	// Fem una cua de posicions i afegim la llum
	cuaLlumNatural.emplace(posLlum);

	while (!cuaLlumNatural.empty()) {
		glm::vec3 bloc = cuaLlumNatural.front();
		cuaLlumNatural.pop();
		uint8_t llum = obtenirLlumNaturalCub(bloc.x, bloc.y, bloc.z);
		posarLlumNatural(bloc + glm::vec3(1, 0, 0), llum);
		posarLlumNatural(bloc + glm::vec3(-1, 0, 0), llum);
		posarLlumNatural(bloc + glm::vec3(0, 1, 0), llum, true);
		posarLlumNatural(bloc + glm::vec3(0, -1, 0), llum, true);
		posarLlumNatural(bloc + glm::vec3(0, 0, 1), llum);
		posarLlumNatural(bloc + glm::vec3(0, 0, -1), llum);

	}
}


void SuperChunk::canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar, bool jugador, glm::vec3* color)
{
	if (x / X < XC && z / Z < YC && (reemplacar || obtenirCub(x, y, z) == AIRE)) {
		//cout << x % X << " " << z % Z << endl;
		Chunk2* chunk = Chunks[x / X][z / Z];
		uint8_t tipusBlocAbans = chunk->obtenirCub(x % X, y, z % Z);
		chunk->canviarCub(x % X, y, z % Z, tipus, reemplacar, color);

		//cout << x << " " << y << " " << z << endl;

		if (tipus == LLUM) {
			// Afegim la llum
			canviarLlumArtificialCub(x, y, z, 14);
			afegirLlum(glm::vec3(x, y, z));
			llistaLlums.push_back(glm::vec3(x, y, z));
		}
		else if (tipus == AIRE) {

			if (tipusBlocAbans == LLUM) {
				treureLlum(glm::vec3(x, y, z), 14);
				llistaLlums.remove(glm::vec3(x, y, z));
			}
			else treureLlum(glm::vec3(x, y, z), 0);

		}
		else {
			// NO ES EFICIENT!!!
			for (const glm::vec3 llum : llistaLlums) {
				treureLlum(llum, 14);
				canviarLlumArtificialCub(llum.x, llum.y, llum.z, 14);
				afegirLlum(llum);
			}
			if(!blocs.getBloc(tipus)->transparent) canviarLlumNaturalCub(x, y, z, 0);
		}

		if(jugador) calculaLlumNatural(x, z);

		//chunk->unCanviat = true;
		//chunk->cubCanviat = glm::vec3(x % X, y, z % Z);
	}
	//cout << *color << " " << (int)tipus << endl;
	//cout << "Chunk: " << x/X << ", " << z/Z << "    " << x % X << ", " << z % Z << endl;
}

void SuperChunk::canviarLlumNaturalCub(int x, int y, int z, uint8_t llum)
{
	if (x / X < XC && z / Z < YC) {
		Chunk2* chunk = Chunks[x / X][z / Z];
		chunk->canviarLlumNaturalCub(x % X, y, z % Z, llum);

	}
}

void SuperChunk::canviarLlumArtificialCub(int x, int y, int z, uint8_t llum)
{
	if (x / X < XC && z / Z < YC) {
		Chunk2* chunk = Chunks[x / X][z / Z];
		chunk->canviarLlumArtificialCub(x % X, y, z % Z, llum);

	}
}

uint8_t SuperChunk::obtenirCub(int x, int y, int z) const
{
	if (x / X < XC && z / Z < YC)
		return Chunks[x / X][z / Z]->obtenirCub(x % X, y, z % Z);
	return 0;
}

uint8_t SuperChunk::obtenirLlumNaturalCub(int x, int y, int z)
{
	if (x / X < XC && z / Z < YC)
		return Chunks[x / X][z / Z]->obtenirLlumNaturalCub(x % X, y, z % Z);
	return 0;
}

uint8_t SuperChunk::obtenirLlumArtificialCub(int x, int y, int z)
{
	if (x / X < XC && z / Z < YC)
		return Chunks[x / X][z / Z]->obtenirLlumArtificialCub(x % X, y, z % Z);
	return 0;
}

void SuperChunk::BoundingBox(int x, int y, int z)
{
	if (!obtenirCub(x, y, z)) return;
	renderer->activaBounding(1);
	x -= X*XC-X;
	z -= Z*YC-Z;
	glBindVertexArray(VAO);
	GLfloat vertices[] = {
	  0.0 + x, 0.0 + y, 0.0 + z, 1.0,
	  1.0 + x, 0.0 + y, 0.0 + z, 1.0,
	  1.0 + x, 1.0 + y, 0.0 + z, 1.0,
	  0.0 + x, 1.0 + y, 0.0 + z, 1.0,
	  0.0 + x, 0.0 + y, 1.0 + z, 1.0,
	  1.0 + x, 0.0 + y, 1.0 + z, 1.0,
	  1.0 + x, 1.0 + y, 1.0 + z, 1.0,
	  0.0 + x, 1.0 + y, 1.0 + z, 1.0,
	};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
	  0, 1, 2, 3,
	  4, 5, 6, 7,
	  0, 4, 1, 5,
	  2, 6, 3, 7
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute
		4,                  // number of elements per vertex, here (x,y,z,w)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);
	renderer->activaBounding(0);
	glBindVertexArray(0);

}

bool SuperChunk::existeixCub(int x, int y, int z, uint8_t tipus) const
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			for (int k = -1; k <= 1; k++)
			{
				if (i == 0 && j == 0 && k == 0) continue;
				if (obtenirCub(x + i, y + j, z + k) == tipus) return true;
			}
		}
	}
	return false;
}

void SuperChunk::update()
{
	
}

void SuperChunk::render()
{	
	if (renderer) {
		renderer->usarShader(0);
		renderer->usarTexturaMon();
		glBindVertexArray(VAO);

		for (int i = 0; i < XC; i++)
		{
			for (int j = 0; j < YC; j++)
			{
				if (Chunks[i][j]) {
					// Hem de moure el chunk per tal que no estiguin tots al mateix lloc
					//glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(i * X - X * XC / 2, -Y/2, j * Z - Z * YC / 2));
					glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(i * X, 0, j * Z));
					renderer->colocarMat4("model", model);
					Chunks[i][j]->render();
				}

			}
		}
		glBindVertexArray(0);
	}
	
}

bool SuperChunk::renderCub(int x, int y, int z)
{
	glBindVertexArray(VAO);
	if (x / X < XC && z / Z < YC) {
		// Hem d'aplicar la mateixa transformació que abans
		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x/X * X, 0, z/Z * Z));
		renderer->colocarMat4("model", model);

		return Chunks[(x / X)][(z / Z)]->renderCub(x % X, y, z % Z);

	}
	return false;
}

void SuperChunk::arbre(int x, int y, int z)
{
	// Tronco
	const int MIN_TRONCO = 5, MAX_TRONCO = 3;
	int max = MIN_TRONCO + (rand() % MAX_TRONCO);
	for (int i = 0; i < max; i++)
	{
		canviarCub(x, y + i, z, FUSTA);
	}
	glm::vec3 *color = Recursos::obtColor("VerdFulles");
	// Copa del arbol
	emplenar(x, y + max, z, 1, 1, FULLES, 0, false,color);
	int segY = y + max - 1;
	float probabilitat = 0.3;
	emplenar(x, segY, z, 1, 1, FULLES, probabilitat, false,color);
	segY--;
	emplenar(x, segY, z, 2, 2, FULLES, probabilitat, false,color);
	segY--;                                                                                                                                                                                                                                              
	emplenar(x, segY, z, 2, 2, FULLES, probabilitat, false,color);
}

void SuperChunk::emplenar(int x, int y, int z, int amplitut, int llargada, uint8_t tipus, float probabilitat, bool reemplacar, glm::vec3* color) {
	for (int i = -amplitut; i <= amplitut; i++) {
		for (int j = -llargada; j <= llargada; j++) {
			if (abs(i) != amplitut || abs(i) != abs(j) || (float)(rand()) / (float)(RAND_MAX) <= probabilitat) {
				canviarCub(x + i, y, z + j, tipus, reemplacar,false,color);
			}
			if (abs(i) != amplitut || abs(i) != abs(j) || (float)(rand()) / (float)(RAND_MAX) <= probabilitat) {
				canviarCub(x - i, y, z - j, tipus, reemplacar,false,color);
			}

		}
	}
}

void SuperChunk::emplenarArea(int x1, int y1, int z1, int x2, int y2, int z2, uint8_t tipus, bool reemplacar, glm::vec3* color)
{
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			for (int k = z1; k <= z2; k++) {
				canviarCub(i,j,k,tipus,reemplacar,false,color);
			}
		}
	}
}
