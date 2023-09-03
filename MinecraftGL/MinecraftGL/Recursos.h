#pragma once

#include "Blocs/Blocs.h"
#include "Items.h"
#include "Renderer/Textura.h"
#include "Mates.h"
using namespace std;

class Recursos
{
public:
	typedef vector<glm::vec3> VecColors;
	typedef map<string, Textura*> Textures;
	typedef vector<string> NomTextures;
	typedef Blocs BLOCS;
	typedef Items ITEMS;

	enum COLORS {
		BLANC,
		VERDGESPA,
		VERDFULLES,
		AIGUA,
		CEL,
		CEL_NIT
	};
	static glm::vec3* obtColor(int color) {
		return &COLORS[color];
	}

	static Bloc* getBloc(int id);
	static Item* getItem(int id);
	static Item* getItem(string nom);

	static Textura* obtTextura(string nom);

	static int width;
	static int height;

	static bool jocAcabat;

	
private:
	static void afegirTextura(Textura* t);
	static VecColors COLORS;
	static BLOCS _blocs;
	static ITEMS _items;
	static map<string, Textura*> _textures;

	static NomTextures texturesPrecarrega;

	Recursos();
	~Recursos() {
		for (auto t : _textures) {
			delete t.second;
		}
	}
};