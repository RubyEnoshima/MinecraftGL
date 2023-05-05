#pragma once
#include "SuperChunk.h"

class Generador
{
public:
	Generador(SuperChunk* sc);
	void marcarArbre(float prob, const glm::vec3 pos);
	void generarArbres();
private:
	const float PROB_ARBRE = 0.01;
	vector<glm::vec3> posArbres;
	SuperChunk* mon;
};

