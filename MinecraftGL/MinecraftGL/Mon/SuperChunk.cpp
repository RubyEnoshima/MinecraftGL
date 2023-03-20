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
}

SuperChunk::SuperChunk(Renderer* _renderer)
{
	renderer = _renderer;
	for (int i = 0; i < XC; i++)
	{
		for (int j = 0; j < YC; j++)
		{
			Chunks[i][j] = new Chunk2(i,j);
		}
	}

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
			Chunks[i][j]->emplenarChunk(llumNatural);
		}
	}

}

void SuperChunk::posarLlum(glm::vec3 pos, uint8_t llum, bool natural, bool avall) {
	// Mirem si és transparent (de moment 0, aire) i si hem de pujar la llum
	if (obtenirCub(pos.x, pos.y, pos.z) == AIRE && ((natural && obtenirLlumNaturalCub(pos.x, pos.y, pos.z) + 2 <= llum) || obtenirLlumArtificialCub(pos.x, pos.y, pos.z) + 2 <= llum)) {
		uint8_t resLlum = llum - 1;
		if (natural && avall) {
			resLlum++;
		}
		if (natural) {
			canviarLlumNaturalCub(pos.x, pos.y, pos.z, resLlum);
		}
		else {
			canviarLlumArtificialCub(pos.x, pos.y, pos.z, resLlum);
		}
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

void SuperChunk::afegirLlum(const glm::vec3 posLlum, bool natural)
{
	// Fem una cua de posicions i afegim la llum
	cuaLlum.emplace(posLlum);

	while (!cuaLlum.empty()) {
		glm::vec3 bloc = cuaLlum.front();
		cuaLlum.pop();
		uint8_t llum;
		if (!natural) llum = obtenirLlumArtificialCub(bloc.x, bloc.y, bloc.z);
		else llum = obtenirLlumNaturalCub(bloc.x, bloc.y, bloc.z);
		posarLlum(bloc + glm::vec3(1, 0, 0), llum, natural);
		posarLlum(bloc + glm::vec3(-1, 0, 0), llum, natural);
		if(!natural)posarLlum(bloc + glm::vec3(0, 1, 0), llum, natural);
		posarLlum(bloc + glm::vec3(0, -1, 0), llum, natural, true);
		posarLlum(bloc + glm::vec3(0, 0, 1), llum, natural);
		posarLlum(bloc + glm::vec3(0, 0, -1), llum, natural);

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

void SuperChunk::canviarCub(int x, int y, int z, uint8_t tipus)
{
	if (x / X < XC && z / Z < YC) {
		Chunk2* chunk = Chunks[x / X][z / Z];
		uint8_t tipusBlocAbans = chunk->obtenirCub(x % X, y, z % Z);
		chunk->canviarCub(x % X, y, z % Z, tipus);
		cout << x << " " << y << " " << z << endl;
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

			if (chunk->cubTop(x%X, y, z%Z)) {
				chunk->canviarLlumNaturalCub(x%X, y, z%Z, 15);
			}
			else {
				uint8_t llumMaxima = chunk->obtenirLlumNaturalMaxima(x%X,y,z%Z);
				if(llumMaxima >= 0) chunk->canviarLlumNaturalCub(x % X, y, z % Z, llumMaxima-1);
				cout << "llum maxima: " << llumMaxima << endl;
			}
		}
		else {
			for (const glm::vec3 llum : llistaLlums) {
				treureLlum(llum, 14);
				canviarLlumArtificialCub(llum.x, llum.y, llum.z, 14);
				afegirLlum(llum);
			}
		}

		//chunk->unCanviat = true;
		//chunk->cubCanviat = glm::vec3(x % X, y, z % Z);
	}
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

uint8_t SuperChunk::obtenirCub(int x, int y, int z)
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

}

void SuperChunk::update()
{
	
}

void SuperChunk::render()
{	
	if (renderer) {

		for (int i = 0; i < XC; i++)
		{
			for (int j = 0; j < YC; j++)
			{
				if (Chunks[i][j]) {
					// Hem de moure el chunk per tal que no estiguin tots al mateix lloc
					//glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(i * X - X * XC / 2, -Y/2, j * Z - Z * YC / 2));
					glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(i * X, -Y/2, j * Z));
					renderer->colocarMat4("model", model);
					Chunks[i][j]->render();
				}

			}
		}
	}
	
}

void SuperChunk::renderCub(int x, int y, int z)
{
	if (x / X < XC && z / Z < YC) {
		// Hem d'aplicar la mateixa transformació que abans
		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x/X * X, -Y / 2, z/Z * Z));
		renderer->colocarMat4("model", model);

		Chunks[(x / X)][(z / Z)]->renderCub(x % X, y, z % Z);

	}
}
