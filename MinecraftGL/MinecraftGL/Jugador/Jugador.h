#pragma once
#include "Camera.h"
#include "Inventari.h"

#define GRAVETAT 9.8
#define SALT 5

enum MODE_JOC {
	SURVIVAL = 0, // Supervivència
	CREATIU = 1, // Per construir lliurement
	ESPECTADOR = 2 // Sense gravetat
};

class Jugador
{
public:
	Jugador(Camera* _camera);
	~Jugador();

	// MOVIMENT
	void update(float deltaTime, const vector<glm::vec3>& blocs);

	void moure(float deltaTime,int tecla);
	void parar();
	void correr();
	void caminar();

	// Retorna true si està colidint
	bool colisiona(const vector<glm::vec3>& blocs);
	bool colisiona(const glm::vec3& bloc);

	// FUNCIONS SOBRE LA POSICIÓ
	// Retorna la posició del jugador
	glm::vec3 obtPos() const;
	// Retorna la posició del jugador en coordenades de bloc al món
	glm::vec3 obtPosBloc() const;
	// Retorna la posició del jugador en coordenades 2D, com si miressim el mon des d'adalt
	glm::vec2 obtPos2D() const;
	// Retorna el chunk on es troba el jugador
	glm::vec2 chunkActual() const;

	Camera* obtCamera() const;

	void canviaMode(int _mode);

	Inventari* inventari;

private:
	int mode = ESPECTADOR; // Veure adalt la llista de modes

	const float velocitat = 7.5;
	float velocitatAct = velocitat;
	glm::vec3 vel;

	vector<glm::vec3> anteriors;
	bool ultimResultat = false;

	Camera* camera = NULL;
};

