#ifndef CHUNK2_H
#define CHUNK2_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include <string.h>
#include <iostream>
#include <vector>
#include "Tipus.h"
#include "glm/gtx/io.hpp"
#include "glm/gtx/string_cast.hpp"

#include <random>
#include <ctime>
#include <chrono>
#include "Blocs/Blocs.h"
#include "Recursos.h"
#include <bitset>
using namespace std;

#define X 16
#define Y 128
#define Z 16

struct Vertex {

};

struct Cub {
	uint8_t tipus = 0;
	uint8_t llum = 0;
	glm::vec3* color = Recursos::obtColor("Blanc");
	bool top = false;
};

class Chunk2
{
public:
	Chunk2(unsigned int _x=0, unsigned int _y=0, Blocs* _blocs=NULL);
	~Chunk2();

	// Canvia el tipus d'un cub concret
	void canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar=true, glm::vec3* color = Recursos::obtColor("Blanc")); 
	// Canvia la llum d'un cub concret
	void canviarLlumNaturalCub(int x, int y, int z, uint8_t llum);
	void canviarLlumArtificialCub(int x, int y, int z, uint8_t llum);
	// Retorna el tipus del cub
	uint8_t obtenirCub(int x, int y, int z) const; 
	// Retorna la llum del cub
	uint8_t obtenirLlumNaturalCub(int x, int y, int z) const;
	uint8_t obtenirLlumArtificialCub(int x, int y, int z) const;

	void update();
	void render();
	bool renderCub(int x, int y, int z);

	int nCubs() const;

	// Genera el chunk amb Perlin noise i posa la llum on toca. Retorna un vector de posicions amb el nombre de l'estructura generada (0: arbre, 1: amapola)
	vector<pair<int, glm::vec3>> emplenarChunk();
	void afegirVeins(Chunk2* left=NULL, Chunk2* right= NULL, Chunk2* up = NULL, Chunk2* down = NULL);

	bool cubTop(int8_t x, int8_t y, int8_t z) const;

	vector<GLbyte> vertices;
	unsigned int elements = 0;

	bool canviat = true; // Ens diu si ha canviat o no el chunk
	bool unCanviat = false;
	glm::vec3 cubCanviat;
private:
	void afegirVertex(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, bool u=0, bool v=0, uint8_t llum = 2, uint8_t costat = 0, glm::vec3* color = Recursos::obtColor("Blanc"));
	void afegirVertexFlat(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, bool r=1, bool g=1, bool b=1);

	void afegirCub(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, glm::vec3* _color);
	void afegirCubFlat(vector<GLbyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus);

	unsigned int VBO;

	int nVertexTipus = 12;

	unsigned int posX, posY;
	Cub chunk[X][Y][Z];

	Chunk2* veiEsq = NULL;
	Chunk2* veiDre = NULL;
	Chunk2* veiUp = NULL;
	Chunk2* veiBaix = NULL;

	//SuperChunk* mon = NULL;

	Blocs* blocs = NULL;

	// Probabilitats de generació: 1 == 100%
	float probabilitatArbre = 0.025;
	float probabilitatFlor = 0.25;
};

#endif