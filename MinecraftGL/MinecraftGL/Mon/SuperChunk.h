#ifndef SUPERCHUNK_H
#define SUPERCHUNK_H
class SuperChunk;
#include "Chunk2.h"
#include "Renderer/Renderer.h"
#include <queue>
#include <list>

#define XC 8
#define YC 8

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

	void calculaLlumNatural(int x, int z);

	// Canvia el tipus d'un cub concret
	void canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar=true, bool jugador=false); 
	// Canvia quanta llum té un cub
	void canviarLlumNaturalCub(int x, int y, int z, uint8_t llum);
	void canviarLlumArtificialCub(int x, int y, int z, uint8_t llum);
	// Retorna el tipus del cub
	uint8_t obtenirCub(int x, int y, int z) const; 
	// Retorna la llum del cub
	uint8_t obtenirLlumNaturalCub(int x, int y, int z);
	uint8_t obtenirLlumArtificialCub(int x, int y, int z);

	void afegirLlum(const glm::vec3 posLlum);
	void treureLlum(const glm::vec3 posLlum, uint8_t llumIni);

	void afegirLlumNatural(const glm::vec3 posLlum);
	void treureLlumNatural(const glm::vec3 posLlum, uint8_t llumIni);

	// Genera un BoundingBox per un cub concret
	void BoundingBox(int x, int y, int z);

	// True si existeix un cub d'un tipus concret al voltant d'un cub
	bool existeixCub(int x, int y, int z, uint8_t tipus) const;

	void update();
	void render();
	// Renderitza un cub en una posició de manera que cada cara es pugui identificar pel color
	bool renderCub(int x, int y, int z);

	// MÉTODES PER GENERAR CUBS
	// Emplena un rectangle centrat en una posició, extenent-se en amplitut i llargada, amb un tipus concret i una probabilitat que les cantonades puguin desapareixer.
	void emplenar(int x, int y, int z, int amplitut, int llargada, uint8_t tipus, float probabilitat = 1, bool reemplacar = true);

	// Emplena un àrea delimitada per dos punts amb un tipus determinat
	void emplenarArea(int x1, int y1, int z1, int x2, int y2, int z2, uint8_t tipus, bool reemplacar = true);

	// VEGETACIÓ I GENERACIÓ
	// Genera un arbre en una posicio concreta
	void arbre(int x, int y, int z);

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

private:
	
	void posarLlum(glm::vec3 pos, uint8_t llum);
	void eliminarLlum(glm::vec3 pos, uint8_t llum);

	void posarLlumNatural(glm::vec3 pos, uint8_t llum, bool avall=false);
	void eliminarLlumNatural(glm::vec3 pos, uint8_t llum);

	Chunk2* Chunks[XC][YC];
	Renderer* renderer;

	unsigned int VAO;

	// GENERACIÓ DE MON

	//int octaves = 8;
	//float frequencia = 8.0;
	//float amplitud = 1.0;
	int semilla = 874;

	const vector<int> flors = { ROSA,DENT_DE_LLEO,TULIPA_TARONJA,ESCLATASANG,XAMPINYO };

	Blocs blocs;


	// Propagació de llums "artificials"
	queue<glm::vec3> llums;
	queue<glm::vec3> cuaLlum;
	queue<pair<glm::vec3, uint8_t>> cuaLlumTreure;
	list<glm::vec3> llistaLlums;

	// Llum natural
	queue<glm::vec3> cuaLlumNatural;

	uint8_t llumNatural = 15;
};

#endif