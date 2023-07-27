#pragma once
#include <vector>
#include "Tipus.h"
#include "Renderer/SpriteRenderer.h"
#include "Renderer/TextRenderer.h"
#include "Recursos.h"
#define MAX_ITEMS 9
#define MAX_FILES 5

// Un Slot de l'inventari. Es guarda l'id i la quantitat d'un item.
class Slot{
public:
	Slot(Textura* t, int _id) {
		id = _id;
		sprite = new Sprite(t, "Slot" + to_string(id), glm::vec2(0,0), glm::vec2(.425), true);
		sprite->tamanyMapa = glm::vec2(16, 16);
		sprite->visible = false;
		sprite->indexZ = 2;
	}
	~Slot() {
		//if (sprite != NULL) delete sprite;
		sprite = NULL;
	}
	
	void actualitzaSprite();
	void setItem(int _item) {
		item = _item;
		actualitzaSprite();
	}
	int obtItem() const {
		return item;
	}

	bool mouseSobre(const glm::vec2& posMouse) const;

	void render(ShaderProgram* shader, int VAO);

	int id = 0; // id de l'slot
	uint8_t quantitat = 0;
	bool visible = false;

	Sprite* sprite = NULL;
private:
	int item = -1; // id de l'item

};

class Inventari
{
public:
	Inventari();
	~Inventari();

	void iniciaSprites(SpriteRenderer* _renderer, TextRenderer* _text);
	// Canvia l'item seleccionat
	void canviaSeleccionat(const uint8_t seleccio);
	void canviaSelecccionatPer1(int seleccio);
	// Retorna l'item seleccionat
	Item* obtenirItemActual() const;
	// Afegeix l'item a l'inventari
	void afegirItem(int id, uint8_t _quantitat = 1);
	// Si no sabem l'id o ens fa molta mandra, podem posar el nom
	void afegirItem(string nom, uint8_t _quantitat = 1); 

	void alternaVisibilitat();

	// Obre i tanca  l'inventari
	void obrir(); 

	void render(const glm::vec2& mousePos);
private:
	bool creatiu = true; // Si true, comencem amb un inventari ple de blocs infinits
	bool visible = true;
	bool dintre = false;

	void initRenderData();

	// Emplena l'inventari amb tots els blocs del json
	void emplenarInventariGran();

	Sprite* spriteSlot = NULL;
	Sprite* caixeta = NULL;
	int slotSeleccionat = 0;
	glm::vec2 posInicial;
	
	vector<Slot*> inventari;
	vector<vector<Slot*>> inventariGran;
	// Quants items diferents hi ha. Hauria de ser menor que MAX_ITEMS
	uint8_t quantitat = 0; 

	SpriteRenderer* renderer;
	TextRenderer* text;
	Textura* mapaItems;

	// Renderitzar blocs
	Textura* mapaBlocs;
	unsigned int VAO = 0, VBO = 0;
	ShaderProgram* shader;
};

