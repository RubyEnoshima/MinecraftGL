#include "Inventari.h"

Inventari::Inventari()
{
	inventari = vector<Item*>(MAX_ITEMS, NULL);
}

Inventari::~Inventari()
{
	for (auto item : inventari)
	{
		delete item;
	}
}

void Inventari::iniciaSprites(SpriteRenderer* renderer)
{
	Sprite* spriteInventari = new Sprite(new Textura("inventari.png"), "Inventari", glm::vec2(), glm::vec2(7));
	spriteInventari->centrat = true;
	spriteInventari->pos = glm::vec2(renderer->width / 2, renderer->height - spriteInventari->tamany.y * 1.7);
	renderer->afegirSprite(spriteInventari);

	spriteSlot = new Sprite(new Textura("seleccio.png"), "Seleccio", glm::vec2(), glm::vec2(7));
	spriteSlot->centrat = true;
	spriteSlot->pos = glm::vec2(renderer->width / 2 - spriteSlot->tamany.x * 2.9 * 4, renderer->height - spriteSlot->tamany.y * 1.7);
	posInicial = spriteSlot->pos;
	spriteSlot->indexZ = 1;
	renderer->afegirSprite(spriteSlot);

}

void Inventari::canviaSeleccionat(const uint8_t seleccio)
{
	if (seleccio >= 0 && seleccio < MAX_ITEMS) {
		slotSeleccionat = seleccio;
		spriteSlot->pos = posInicial;
		spriteSlot->pos.x += seleccio * spriteSlot->tamany.x*2.9;

	}
}
