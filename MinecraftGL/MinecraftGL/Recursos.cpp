#include "Recursos.h"

Recursos::MapColors Recursos::COLORS = {
	{"Blanc",glm::vec3(255)},
	{"VerdGespa",glm::vec3(131, 187, 109)},
	{"VerdFulles",glm::vec3(99, 169, 72)},
	{"Aigua",glm::vec3(63, 118, 228)}
};

Recursos::NomTextures Recursos::texturesPrecarrega = { "icon.png","icons.png","items.png","inventari.png","minecraft_transp.png","nuvols.png","seleccio.png" };

int Recursos::width = 0;
int Recursos::height = 0;

Recursos::BLOCS Recursos::_blocs = Blocs();
Recursos::ITEMS Recursos::_items = Items();
Recursos::Textures Recursos::_textures = map<string, Textura*>();

Bloc* Recursos::getBloc(int id)
{
	return _blocs.getBloc(id);
}

Item* Recursos::getItem(int id)
{
	return _items.getItem(id);
}

Item* Recursos::getItem(string nom)
{
	return _items.getItem(nom);
}

Textura* Recursos::obtTextura(string nom)
{
	if (_textures.empty()) {
		for (string nom : texturesPrecarrega)
		{
			afegirTextura(new Textura(nom));
		}
	}
	return _textures[nom];
}

void Recursos::afegirTextura(Textura* t)
{
	_textures.insert({ t->nom,t });
}
