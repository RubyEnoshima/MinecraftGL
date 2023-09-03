#include "Recursos.h"

Recursos::VecColors Recursos::COLORS = {
	glm::vec3(255), // Blanc
	glm::vec3(131, 187, 109), // Verd gespa
	glm::vec3(99, 169, 72), // Verd fulles
	glm::vec3(63, 118, 228), // Aigua
	glm::vec3(110, 170, 255), // Cel
	glm::vec3(10, 31, 61), // Cel de nit

};

Recursos::NomTextures Recursos::texturesPrecarrega = { "icon.png","icons.png","items.png","inventari.png","minecraft_transp.png","nuvols.png","seleccio.png","creatiu.png", "caixeta.png"};

int Recursos::width = 0;
int Recursos::height = 0;
bool Recursos::jocAcabat = false;

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
