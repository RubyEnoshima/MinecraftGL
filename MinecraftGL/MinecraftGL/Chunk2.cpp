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
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(0);
	vertices.push_back(0);
}

glm::vec3 Chunk2::calcularNormal(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2)
{
	glm::vec3 A = P1 - P0, B = P2-P1;
	glm::vec3 N(A.y*B.z-A.z*B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
	return glm::normalize(N);
}

void Chunk2::BoundingBox(int8_t x, int8_t y, int8_t z)
{
	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
	  0.0+x, 0.0+y, 0.0+z, 1.0,
	  1.0+x, 0.0+y, 0.0+z, 1.0,
	  1.0+x, 1.0+y, 0.0+z, 1.0,
	  0.0+x, 1.0+y, 0.0+z, 1.0,
	  0.0+x, 0.0+y, 1.0+z, 1.0,
	  1.0+x, 0.0+y, 1.0+z, 1.0,
	  1.0+x, 1.0+y, 1.0+z, 1.0,
	  0.0+x, 1.0+y, 1.0+z, 1.0,
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
	vector<GLbyte> _vertices;

	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				uint8_t tipus = chunk[i][j][k];
				if(tipus) afegirCub(_vertices,i,j,k);
			}
		}
	}

	elements = _vertices.size();
	vertices = _vertices.data();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, elements, vertices, GL_STATIC_DRAW);
	
	canviat = false;
}

void Chunk2::render()
{
	if (canviat) update();


	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 8 * sizeof(GLbyte), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 8 * sizeof(GLbyte), (void*)(3 * sizeof(GLbyte)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_BYTE, GL_FALSE, 8 * sizeof(GLbyte), (void*)(6 * sizeof(GLbyte)));
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, elements);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

int Chunk2::nCubs() const
{
	return elements/6;
}

void Chunk2::emplenarChunk()
{
	for (int i = 0; i < X; i++) {
		for (int j = Y/2; j < Y; j++) {
			for (int k = 0; k < Z; k++) {
				canviarCub(i,j,k,0);
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

