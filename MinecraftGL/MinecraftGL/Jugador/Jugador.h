#pragma once
#include "Camera.h"
#include "Inventari.h"

#define GRAVETAT 20
#define SALT 7

enum MODE_JOC {
	SURVIVAL = 0, // Supervivència
	CREATIU = 1, // Per construir lliurement
	ESPECTADOR = 2 // Sense gravetat
};

class Jugador
{
public:
	Jugador(Camera* _camera, ShaderProgram* shader);
	~Jugador();

	// MOVIMENT
	void update(float deltaTime, const vector<AABB>& blocs);

	void moure(const map<int, bool>& tecles);
	void parar();
	void correr();
	void caminar();

	// FUNCIONS SOBRE LA POSICIÓ
	// Retorna la posició del jugador
	glm::vec3 obtPos() const;
	// Retorna la posició del jugador en coordenades de bloc al món
	glm::vec3 obtPosBloc(bool peu = true) const;
	// Retorna la posició del jugador en coordenades 2D, com si miressim el mon des d'adalt
	glm::vec2 obtPos2D() const;
	// Retorna el chunk on es troba el jugador
	glm::vec2 chunkActual() const;

	Camera* obtCamera() const;

	void canviaMode(int _mode);

	Inventari* inventari;

private:
	int mode = ESPECTADOR; // Veure adalt la llista de modes
	const float velocitat = 12;
	vector<AABB> _blocs;

	bool enTerra = false;

	AABB aabb;

	float velocitatAct = velocitat;
	glm::vec3 vel;

	Camera* camera = NULL;
};

