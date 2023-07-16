#pragma once
#include "Renderer/SpriteRenderer.h"
#include "Renderer/TextRenderer.h"
#include "Inventari.h"
class HUD
{
public:
	HUD(Renderer* _renderer, Inventari* _inventari);
	~HUD();

	void render();
	void alternaVisibilitat();
private:
	void iniciaSprites();

	bool visible = true;

	SpriteRenderer* renderer;
	TextRenderer* textRenderer;
};

