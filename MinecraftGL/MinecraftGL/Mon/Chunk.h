#ifndef CHUNK2_H
#define CHUNK2_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include "FastNoiseLite.h"

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

using namespace std;

#define X 16
#define Y 128
#define Z 16

struct Soroll {
	FastNoiseLite* noise;
	vector<glm::vec2> punts;
};

struct Cub {
	uint8_t tipus = 0;
	uint8_t llum = 0;
	char* color;
	bool top = false;
};

class Chunk
{
public:
	Chunk(unsigned int _x=0, unsigned int _y=0);
	~Chunk();

	// Canvia el tipus d'un cub concret
	void canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar=true, char* color = (char*)"Blanc"); 
	// Canvia la llum d'un cub concret
	void canviarLlumNaturalCub(int x, int y, int z, uint8_t llum);
	void canviarLlumArtificialCub(int x, int y, int z, uint8_t llum);
	// Retorna el tipus del cub
	uint8_t obtenirCub(int x, int y, int z) const; 
	// Retorna la llum del cub
	uint8_t obtenirLlumNaturalCub(int x, int y, int z) const;
	uint8_t obtenirLlumArtificialCub(int x, int y, int z) const;

	void crearVertexs();
	void update();
	void render(bool semi = false);
	bool esVisible(const glm::mat4& mvp) const;
	bool esVisible(const vector<Pla>& plansFrustum) const;
	bool renderCub(int x, int y, int z);

	int nCubs() const;
	glm::vec2 obtPos() const;

	// Genera el chunk amb Perlin noise i posa la llum on toca. Retorna un vector de posicions amb el nombre de l'estructura generada (0: arbre, 1: amapola)
	vector<pair<int, glm::vec3>> emplenarChunk(int tipus, const vector<Soroll>& noises);
	void afegirVeins(Chunk* left=NULL, Chunk* right= NULL, Chunk* up = NULL, Chunk* down = NULL);

	bool cubTop(int8_t x, int8_t y, int8_t z) const;

	vector<GLubyte> _vertices, _vertices_transp;
	unsigned int elements = 0, elements_transp = 0;

	bool canviat = true; // Ens diu si ha canviat o no el chunk
	bool unCanviat = false;
	uint8_t veiCanviat = -1; 

	bool descarregant = false;
	bool preparat = false;
	bool carregat = false;
	bool generat = false;

	glm::vec3 cubCanviat;
private:
	void afegirVertex(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, bool u=0, bool v=0, uint8_t llum = 2, uint8_t costat = 0, const char* color = "Blanc");
	void afegirVertexFlat(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, bool r=1, bool g=1, bool b=1);

	void afegirCub(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus, const char* _color);
	void afegirCubFlat(vector<GLubyte>& vertices, int8_t x, int8_t y, int8_t z, uint8_t tipus);

	unsigned int VBO = 0, VBO_TRANSP = 0;

	vector<glm::vec3> cantonades;

	int posX, posY;
	Cub chunk[X][Y][Z];

	Chunk* veiEsq = NULL;
	Chunk* veiDre = NULL;
	Chunk* veiUp = NULL;
	Chunk* veiBaix = NULL;

	// Probabilitats de generació: 1 == 100%
	float probabilitatArbre = 0.025;
	float probabilitatFlor = 0;
};

#endif