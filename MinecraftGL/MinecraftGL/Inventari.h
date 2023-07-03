#pragma once
#include "Item.h"
#include <vector>

#define MAX_ITEMS 9

class Inventari
{
public:
	Inventari();
	~Inventari();

	void iniciaSprites(SpriteRenderer* renderer);
	void canviaSeleccionat(const uint8_t seleccio);
private:
	Sprite* spriteSlot;
	uint8_t slotSeleccionat = 0;
	glm::vec2 posInicial;
	vector<Item*> inventari;
};

