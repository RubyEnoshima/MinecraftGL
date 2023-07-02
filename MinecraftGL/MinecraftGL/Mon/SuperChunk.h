#ifndef SUPERCHUNK_H
#define SUPERCHUNK_H

#include "Chunk.h"
#include "Renderer/Renderer.h"
#include <queue>
#include <deque>
#include <list>
#include <set>
#include <mutex>

//#define XC 3
//#define YC 3

//#define SIZE 11
#define DISTANCIA 13
#define NCHUNKS 4 // Quants chunks pot processar en un sol frame

#define DEBUG true // true: La llum natural no es calcularà
#define DEBUG_FRUSTUM true // true: apliquem Frustum Culling

const struct CompararVec2 {
	bool operator()(const glm::vec2& a, const glm::vec2& b) const {
		if (a.x < b.x) {
			return true;
		}
		if (a.x > b.x) {
			return false;
		}
		return a.y < b.y;
	}
};

struct Ray {
	glm::vec3 origen;
	glm::vec3 direccion;
};

class SuperChunk
{
public:
	SuperChunk();
	~SuperChunk();
	SuperChunk(Renderer* _renderer);

	void comprovarChunks(const glm::vec2& chunkJugador);
	void descarregarChunks();
	void carregarChunks();
	void eliminaCarregats();
	void update(const glm::vec2& chunkJugador, const glm::mat4& mvp = glm::mat4());
	void render(const vector<Pla>& mvp);

	float tempsCarrega = 0;


	void generarChunk(const glm::vec2& pos, vector<glm::vec3>&arbrets);

	// Canvia el tipus d'un cub concret
	void canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar=true, bool jugador=false, char* color = (char *)"Blanc");
	// Canvia quanta llum té un cub
	void canviarLlumNaturalCub(int x, int y, int z, uint8_t llum);
	void canviarLlumArtificialCub(int x, int y, int z, uint8_t llum);
	// Retorna el tipus del cub
	uint8_t obtenirCub(const glm::vec3&pos) const;
	uint8_t obtenirCub(int x, int y, int z) const;

	// ILUMINACIO

	// Retorna la llum del cub
	uint8_t obtenirLlumNaturalCub(int x, int y, int z);
	uint8_t obtenirLlumArtificialCub(int x, int y, int z);

	void afegirLlum(const glm::vec3 posLlum);
	void treureLlum(const glm::vec3 posLlum, uint8_t llumIni);

	void afegirLlumNatural(const glm::vec3 posLlum);
	void treureLlumNatural(const glm::vec3 posLlum, uint8_t llumIni);

	void calculaLlumNatural(const glm::vec2& pos);
	void calculaLlumNatural(int x, int z);

	// Genera un BoundingBox per un cub concret
	void BoundingBox(int x, int y, int z);

	// True si existeix un cub d'un tipus concret al voltant d'un cub
	bool existeixCub(int x, int y, int z, uint8_t tipus) const;

	// Renderitza un cub en una posició de manera que cada cara es pugui identificar pel color
	bool renderCub(int x, int y, int z);

	// MÉTODES PER GENERAR CUBS
	// Emplena un rectangle centrat en una posició, extenent-se en amplitut i llargada, amb un tipus concret i una probabilitat que les cantonades puguin desapareixer.
	void emplenar(int x, int y, int z, int amplitut, int llargada, uint8_t tipus, float probabilitat = 1, bool reemplacar = true, char* color = (char*)"Blanc");

	// Emplena un àrea delimitada per dos punts amb un tipus determinat
	void emplenarArea(int x1, int y1, int z1, int x2, int y2, int z2, uint8_t tipus, bool reemplacar = true, char* color = (char*)"Blanc");

	// VEGETACIÓ I GENERACIÓ
	// Genera un arbre en una posicio concreta
	void arbre(int x, int y, int z);

	// RAYCAST
	glm::vec3 raycast(const Ray& ray,const glm::vec3& pos) const{
		float minDistance = std::numeric_limits<float>::max();
		glm::vec3 closestVoxel;
		int rango = 10;
		for (int i = -rango; i < rango; i++) {
			for (int j = -rango; j < rango; j++) {
				for (int k = -rango; k < rango; k++) {
					float t = intersectRayVoxel(ray, glm::vec3(pos.x+i, pos.y + j, pos.z + k));
					if (t > 0 && t < minDistance) {
						minDistance = t;
						closestVoxel = glm::vec3(pos.x + i, pos.y + j, pos.z + k);
					}
				}
			}
		}
			
		

		return closestVoxel;
	}

	float intersectRayVoxel(const Ray& ray, const glm::vec3& voxel) const{
		if (!obtenirCub(voxel.x, voxel.y, voxel.z)) return -1;
		glm::vec3 tMin = (voxel - ray.origen) / ray.direccion;
		glm::vec3 tMax = (voxel + glm::vec3(1.0f) - ray.origen) / ray.direccion;

		glm::vec3 tEnter = glm::min(tMin, tMax);
		glm::vec3 tExit = glm::max(tMin, tMax);

		float tNear = glm::max(glm::max(tEnter.x, tEnter.y), tEnter.z);
		float tFar = glm::min(glm::min(tExit.x, tExit.y), tExit.z);

		if (tNear <= tFar && tFar >= 0) {
			return tNear;
		}
		else {
			return -1.0f;
		}
	}

	// Retorna tots els cubs valids (transparents)
	vector<glm::vec3> obtenirColindants(const glm::vec3& pos, bool transparents = false, bool ellMateix = false) const;

	bool potGenerar = true;
	bool carregat = false;

	Blocs blocs;


private:
	int tipusMon = Recursos::PLA;

	glm::vec2 chunkInicial;

	void posarLlum(glm::vec3 pos, uint8_t llum);
	void eliminarLlum(glm::vec3 pos, uint8_t llum);

	void posarLlumNatural(glm::vec3 pos, uint8_t llum, bool avall=false);
	void eliminarLlumNatural(glm::vec3 pos, uint8_t llum);

	// FUNCIONS PER BLOCS
	

	bool esValid(int x, int y, int z) const;
	bool esValid(const glm::vec3& pos) const;

	// Ens diu en quin chunk es troba un bloc
	glm::vec2 BlocChunk(const glm::vec3& pos) const;
	glm::vec2 BlocChunk(int x, int z) const;
	int Mon2Chunk(int n, int m) const; // Converteix una coordenada en mon (podria ser -79, 980...) a coordenada de chunk (-79 => 1, 980 => 4)

	mutable map<glm::vec2,Chunk*,CompararVec2> Chunks;

	Renderer* renderer;

	unsigned int VAO;

	// GENERACIÓ DE MON

	//int octaves = 8;
	//float frequencia = 8.0;
	//float amplitud = 1.0;
	int semilla = 874;

	const vector<int> flors = { ROSA,DENT_DE_LLEO,TULIPA_TARONJA,ESCLATASANG,XAMPINYO };

	// Propagació de llums "artificials"
	queue<glm::vec3> llums;
	queue<glm::vec3> cuaLlum;
	queue<pair<glm::vec3, uint8_t>> cuaLlumTreure;
	list<glm::vec3> llistaLlums;

	// Llum natural
	queue<glm::vec3> cuaLlumNatural;

	uint8_t llumNatural = 15;

	glm::vec3 posicions[7] = { glm::vec3(-1,0,0),glm::vec3(0,-1,0),glm::vec3(0,0,-1),glm::vec3(1,0,0),glm::vec3(0,1,0),glm::vec3(0,0,1),glm::vec3(0,0,0) };
	
	// GESTIÓ DE CHUNKS
	mutable std::recursive_mutex loadedChunksMutex;
	mutable std::recursive_mutex descarregarMutex;
	mutable std::recursive_mutex cuaMutex;
	deque<Chunk*> chunksDescarregar;
	deque<glm::vec2> chunksCarregar;
	deque<pair<glm::vec2,Chunk*>> ChunksReady;

	bool esCarregat(const glm::vec2& pos) const;
	bool existeixCua(const deque<glm::vec2>& cua, const glm::vec2& e) const;
};

#endif