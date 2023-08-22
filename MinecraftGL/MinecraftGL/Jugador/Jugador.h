#pragma once
#include "Camera.h"
#include "Inventari.h"

#define GRAVETAT 20
#define SALT 7

enum MODE_JOC {
	SURVIVAL = 0, // Superviv�ncia
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
	void render();

	void moure(float deltaTime, const map<int, bool>& tecles);
	void parar();
	void correr();
	void caminar();

	// Retorna true si est� colidint
	bool colisiona(const vector<pair<glm::vec3, uint8_t>>& blocs);
	bool colisiona(const glm::vec3& bloc);
	bool sotaAigua(const vector<pair<glm::vec3, uint8_t>>& blocs) const;

	// FUNCIONS SOBRE LA POSICI�
	// Retorna la posici� del jugador
	glm::vec3 obtPos() const;
	// Retorna la posici� del jugador en coordenades de bloc al m�n
	glm::vec3 obtPosBloc(bool peu = true) const;
	// Retorna la posici� del jugador en coordenades 2D, com si miressim el mon des d'adalt
	glm::vec2 obtPos2D() const;
	// Retorna el chunk on es troba el jugador
	glm::vec2 chunkActual() const;

	Camera* obtCamera() const;

	void canviaMode(int _mode);

	Inventari* inventari;

private:
	int mode = ESPECTADOR; // Veure adalt la llista de modes
	const float velocitat = 10;
	vector<AABB> _blocs;

	bool enTerra = false;

	// Actualitza la posici� de la c�mera segons la velocitat actual
	void actualitzaCamera(float deltaTime);

	AABB aabb;

	float velocitatAct = velocitat;
	glm::vec3 vel;
	glm::vec3 antPos;

	vector<glm::vec3> anteriors;
	bool ultimResultat = false;

	Camera* camera = NULL;
};

