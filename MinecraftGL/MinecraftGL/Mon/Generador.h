#pragma once 
#include <vector>
#include <glm/glm.hpp>
#include <time.h>

#include "FastNoiseLite.h"
#include "Tipus.h"
#include "Mates.h"
using namespace std;

#define LLAVOR_RANDOM false // true: la llavor per generar el mapa serà escollida a l'atzar

enum TIPUS_MON {
	NORMAL,
	PLA
};

struct Soroll {
	FastNoiseLite* noise;
	vector<glm::vec2> punts;
	float importancia = 1;
};

class Generador {
public:
	Generador();
	~Generador();

	int obtFlor() const; // Retorna una flor random del vector de flors
	int obtAltura(float x, float y) const; // Retorna la altura del terreny en x i en y barrejant els sorolls
	int obtTipus(int altura, int max) const; // Retorna el tipus de bloc que hi hauria d'haver en altura

	const int tipusMon = NORMAL;
	int nivellMar = 60;
	int aigua = AIGUA; // Quin tipus de bloc s'utilitzarà per l'aigua
	int nivellNeu = 90;

	// Probabilitats de generació: 1 == 100%
	float probabilitatArbre = 0.015;
	float probabilitatFlor = 0.10;
private:

	int llavor = 874;
	vector<Soroll> noises; // Conjunt de soroll que es pot barrejar.

	// Tipus de flors que poden apareixer al món
	const vector<int> flors = { ROSA,TULIPA_TARONJA,ARBUST }; //ESCLATASANG,XAMPINYO,DENT_DE_LLEO

	// Llista de capes. Baixa, mitja, mitja-alta, platges, alta, alta (altura neu)
	const vector<int> nivells = { BEDROCK, PEDRA, TERRA, SORRA, GESPA, NEU };

	// Donat un segment [a,b], retorna el valor de y donat x
	static double interpolar(const glm::vec2& a, const glm::vec2& b, double x) {
		return a.y + ((b.y - a.y) / (b.x - a.x)) * (x - a.x);
	}

	// Donats diversos punts en un vector, retorna la interpolació del punt
	static double interpolarSegments(const vector<glm::vec2>& punts, double punt) {
		if (punts.size() < 2) return 0;

		for (size_t i = 0; i < punts.size() - 1; i++) {
			if (punt >= punts[i].x && punt <= punts[i + 1].x) {
				return interpolar(punts[i], punts[i + 1], punt);
			}
		}

		return 0;
	}
};