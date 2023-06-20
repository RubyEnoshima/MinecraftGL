#pragma once
#include "Camera.h"

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

	void moure(float deltaTime,int tecla);
	void correr();
	void caminar();

	Camera* obtCamera() const;
	// Retorna la posició del jugador respecte al chunk actual
	glm::vec2 chunkActual() const;
private:
	const float velocitat = 5;
	float velocitatAct = velocitat;

	int mode = ESPECTADOR; // Veure adalt la llista de modes

	Camera* camera = NULL;
};

