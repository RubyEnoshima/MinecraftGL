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
			Chunks[i][j]->emplenarChunk();
		}
	}

}

vector<glm::vec3> SuperChunk::obtenirColindants(glm::vec3& pos) const
{
	vector<glm::vec3> cubs;
	cubs.push_back(pos + glm::vec3(0, 0, 1));
	cubs.push_back(pos + glm::vec3(0, 0, -1));
	cubs.push_back(pos + glm::vec3(-1, 0, 0));
	cubs.push_back(pos + glm::vec3(1, 0, 0));
	/*cubs.push_back(pos + glm::vec3(0, 1, 0));
	cubs.push_back(pos + glm::vec3(0, -1, 0));*/
	return cubs;
}

uint8_t SuperChunk::obtenirLlumMaxima(const vector<glm::vec3>& cubs)
{
	uint8_t max = 0;
	for (const glm::vec3& cub : cubs) {
		uint8_t llum = obtenirLlumCub(cub.x, cub.y, cub.z);
		if (llum > max) max = llum;
	}
	return max;
}

void SuperChunk::calcularLlum()
{
	// RESETEAR LUZ DE TODO EL MUNDO

	// Per cada llum al món...
	for(glm::vec3& llum : llums){
		vector<glm::vec3> cubs = obtenirColindants(llum);

		//while (!cubs.empty()) {
			glm::vec3 cubActual = cubs.back();
			cout << cubActual.x << " " << cubActual.y << " " << cubActual.z << endl;
			// Si és transparent i no iluminat
			if (!obtenirCub(cubActual.x, cubActual.y, cubActual.z) && !obtenirLlumCub(cubActual.x, cubActual.y, cubActual.z)) {
				// Canviarllumcub...
			}
			
		//}
		
	}
}

void SuperChunk::canviarCub(int x, int y, int z, uint8_t tipus)
{
	if (Chunks[x / X][z / Z]) {
		Chunk2* chunk = Chunks[x / X][z / Z];
		chunk->canviarCub(x % X, y, z % Z, tipus);

		if (tipus == LLUM) {
			// Afegim la llum
			llums.push_back(glm::vec3(x, y, z));
			canviarLlumCub(x, y, z, 14);

			calcularLlum();
		}

		//chunk->unCanviat = true;
		//chunk->cubCanviat = glm::vec3(x % X, y, z % Z);
	}
	//cout << "Chunk: " << x/X << ", " << z/Z << "    " << x % X << ", " << z % Z << endl;
}

void SuperChunk::canviarLlumCub(int x, int y, int z, uint8_t llum)
{
	if (Chunks[x / X][z / Z]) {
		Chunk2* chunk = Chunks[x / X][z / Z];
		chunk->canviarLlumCub(x % X, y, z % Z, llum);

	}
}

uint8_t SuperChunk::obtenirCub(int x, int y, int z)
{
	if (Chunks[x / X][z / Z])
		return Chunks[x / X][z / Z]->obtenirCub(x % X, y, z % Z);
	return 0;
}

uint8_t SuperChunk::obtenirLlumCub(int x, int y, int z)
{
	if (Chunks[x / X][z / Z])
		return Chunks[x / X][z / Z]->obtenirCub(x % X, y, z % Z);
	return 0;
}

void SuperChunk::BoundingBox(int x, int y, int z)
{
	//if (!obtenirCub(x, y, z)) return;
	x -= 32;
	z -= 32;
	
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
	if (Chunks[x / X][z / Z]) {
		// Hem d'aplicar la mateixa transformació que abans
		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x/X * X, -Y / 2, z/Z * Z));
		renderer->colocarMat4("model", model);

		Chunks[(x / X)][(z / Z)]->renderCub(x % X, y, z % Z);

	}
}
