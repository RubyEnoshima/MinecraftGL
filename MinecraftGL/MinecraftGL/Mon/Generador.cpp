#include "Generador.h"

Generador::Generador(SuperChunk* sc)
{
	mon = sc;
}

void Generador::marcarArbre(float prob, const glm::vec3 pos)
{
	if (prob > PROB_ARBRE) return;

}

void Generador::generarArbres()
{
}
