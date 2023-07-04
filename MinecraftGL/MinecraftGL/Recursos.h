#pragma once
//#include <glm/glm.hpp>
//#include <string>
//#include <map>
#include "Blocs/Blocs.h"
#include "Items.h"
#include "Renderer/Textura.h"

using namespace std;

struct Pla {
public:
	Pla(const glm::vec3& _pos, const glm::vec3& _normal) {
		normal = _normal;
		pos = _pos;
	}
	glm::vec3 normal = { 0, 0, 0 };
	glm::vec3 pos = { 0,0,0 };

	float distancia(const glm::vec3& p) const{
		return glm::dot(normal, (p - pos));
	}
};

class Recursos
{
public:
	typedef map<string, glm::vec3> MapColors;
	typedef map<string, Textura*> Textures;
	typedef Blocs BLOCS;
	typedef Items ITEMS;
	enum TIPUS_MON {
		NORMAL = 0,
		PLA = 1
	};
	static glm::vec3* obtColor(string color) {
		return &COLORS[color];
	}
	
	bool start = false;

	static Bloc* getBloc(int id);
	static Item* getItem(int id);

	static void afegirTextura(Textura* t);

private:
	static MapColors COLORS;
	static BLOCS _blocs;
	static ITEMS _items;
	static map<string, Textura*> _textures;

	Recursos() { 
	}
	~Recursos() {
		for (auto t : _textures) {
			delete t.second;
		}
	}
};

