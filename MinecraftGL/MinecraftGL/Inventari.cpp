#include "Inventari.h"

Inventari::Inventari()
{
	mapaItems = new Textura("items.png");

	for (int i = 0; i < MAX_ITEMS; i++) {
		inventari.push_back(new Slot(mapaItems,i));
	}
	for (int i = 0; i < MAX_FILES; i++) {
		inventariGran.push_back(vector<Slot*>());
		for (int j = 0; j < MAX_ITEMS; j++) {
			inventariGran[i].push_back(new Slot(mapaItems,10*i+1 + i*j+j));
		}
	}
	glGenBuffers(1, &VAO);
}

Inventari::~Inventari()
{
	for (auto slot : inventari)
	{
		delete slot;
	}
	for (auto vector : inventariGran)
	{
		for (auto slot : vector)
		{
			delete slot;
		}
	}
	glDeleteBuffers(1, &VAO);
	//delete mapaItems;
}

void Inventari::iniciaSprites(SpriteRenderer* renderer)
{
	Sprite* spriteInventari = new Sprite(new Textura("inventari.png"), "Inventari", glm::vec2(), glm::vec2(7));
	spriteInventari->centrat = true;
	spriteInventari->pos = glm::vec2(renderer->width / 2, renderer->height - spriteInventari->tamany.y * 1.7);
	renderer->afegirSprite(spriteInventari);

	spriteSlot = new Sprite(new Textura("seleccio.png"), "Seleccio", glm::vec2(), glm::vec2(7));
	spriteSlot->centrat = true;
	spriteSlot->pos = glm::vec2(renderer->width / 2 - 80 * 3.5, renderer->height - spriteSlot->tamany.y * 1.7);
	posInicial = spriteSlot->pos;
	spriteSlot->indexZ = 1;
	renderer->afegirSprite(spriteSlot);

	for (auto slot : inventari)
	{
		slot->sprite->pos = glm::vec2(renderer->width / 2 - 70 * 4 + 70 * slot->id, renderer->height - 32 - 5);
		renderer->afegirSprite(slot->sprite);
	}
	for (auto vector : inventariGran)
	{
		for (auto slot : vector)
		{
			renderer->afegirSprite(slot->sprite);
		}
	}
}

void Inventari::canviaSeleccionat(const uint8_t seleccio)
{
	if (seleccio >= 0 && seleccio < MAX_ITEMS) {
		slotSeleccionat = seleccio;
		spriteSlot->pos = posInicial;
		spriteSlot->pos.x += seleccio * (70);

	}
}

Item* Inventari::obtenirItemActual() const
{
	return Recursos::getItem(inventari[slotSeleccionat]->item);
}

void Inventari::afegirItem(int id, uint8_t _quantitat)
{
	if (quantitat == MAX_ITEMS) return;
	int i = 0;
	while (i < MAX_ITEMS) {
		Slot* actual = inventari[i];
		if (actual->item == id) {
			actual->quantitat += _quantitat;
			break;
		}
		else if (actual->item == -1) {
			actual->item = id;
			actual->quantitat = _quantitat;
			actual->actualitzaSprite();
			break;
		}
		i++;
	}
	quantitat++;
}

void Slot::actualitzaSprite()
{
	if (item == -1) {
		sprite->visible = false;
		return;
	}
	uint8_t posSprite = Recursos::getItem(item)->sprite;
	sprite->posicioMapa = glm::vec2((posSprite % 16) * 16, (int)(posSprite / 16) * 16);
	sprite->visible = true;
}
