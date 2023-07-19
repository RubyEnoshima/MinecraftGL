#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <glm/glm.hpp>

using json = nlohmann::json;
using namespace std;

#define MAX_ITEMS 256

enum TIPUS_ITEM {
	BLOC=0,
	EINA=1,
	MENJAR=2
};

struct Item
{
	Item() {
		id = -1;
		classe = BLOC;
		nom = "";
		sprite = 0;
		bloc_id = -1;

	}
	Item(int _id, int _classe, string _nom, uint8_t _sprite, uint8_t _bloc_id) {
		id = _id;
		classe = _classe;
		nom = _nom;
		sprite = _sprite;
		bloc_id = _bloc_id;
	}

	int id = -1;
	int classe = BLOC;
	uint8_t bloc_id = -1;
	string nom = "";
	uint8_t sprite = 0; // Al mapa d'items, quin lloc ocupa
};

class Items
{
public:
	Items();
	~Items();
	Item* getItem(int id) const {
		if (id == -1) return NULL;
		return dades.at(id);
	}
	Item* getItem(string nom) const {
		for (Item* i : dades) {
			if (i->nom == nom) {
				return i;
			}
		}
		return NULL;
	}

private:
	vector<Item*> dades;
};
