#pragma once
#include "Camera.h"

#define GRAVETAT 0.5

enum MODE_JOC {
	SURVIVAL = 0,
	CREATIU = 1,
	ESPECTADOR = 2
};

class Jugador
{
public:
	Jugador(Camera* _camera);
	~Jugador();

	void update(float deltaTime);

	void moure(float deltaTime,int tecla);
	void correr();
	void caminar();

	glm::vec3 obtPos() const;
	glm::vec2 obtPos2D() const;

	Camera* obtCamera() const;
	// Retorna la posició del jugador respecte al chunk actual
	glm::vec2 chunkActual() const;
private:
	const float velocitat = 7.5;
	float velocitatAct = velocitat;
	glm::vec2 vel;
	int mode = ESPECTADOR; // Veure adalt la llista de modes

	Camera* camera = NULL;
};

