#pragma once
#include <vector>
#include "Renderer/SpriteRenderer.h"
#include "Recursos.h"
#define MAX_ITEMS 9
#define MAX_FILES 3

// Un Slot de l'inventari. Es guarda l'id i la quantitat d'un item.
struct Slot{
	Slot(Textura* t, int _id) {
		id = _id;
		sprite = new Sprite(t, "Slot" + to_string(id), glm::vec2(0,0), glm::vec2(.425), true);
		sprite->tamanyMapa = glm::vec2(16, 16);
		sprite->visible = false;

	}
	~Slot() {
		//if (sprite != NULL) delete sprite;
		sprite = NULL;
	}
	
	void actualitzaSprite();

	int id = 0;
	int item = -1; // id de l'item
	uint8_t quantitat = 0;

	Sprite* sprite = NULL;
};

class Inventari
{
public:
	Inventari();
	~Inventari();

	void iniciaSprites(SpriteRenderer* renderer);
	// Canvia l'item seleccionat
	void canviaSeleccionat(const uint8_t seleccio);
	void canviaSelecccionatPer1(int seleccio);
	// Retorna l'item seleccionat
	Item* obtenirItemActual() const;
	// Afegeix l'item a l'inventari
	void afegirItem(int id, uint8_t _quantitat = 1);
	// Si no sabem l'id o ens fa molta mandra, podem posar el nom
	void afegirItem(string nom, uint8_t _quantitat = 1); 

	void render();
private:
	void initRenderData();

	Sprite* spriteSlot = NULL;
	int slotSeleccionat = 0;
	glm::vec2 posInicial;
	
	vector<Slot*> inventari;
	vector<vector<Slot*>> inventariGran;
	// Quants items diferents hi ha. Hauria de ser menor que MAX_ITEMS
	uint8_t quantitat = 0; 

	Textura* mapaItems;
	unsigned int VAO = 0, VBO = 0;
	ShaderProgram* shader; // S'utilitza pels blocs
};

