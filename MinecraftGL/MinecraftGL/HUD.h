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
	void modeDebug();
private:
	void iniciaSprites();
	void renderDebug();

	bool visible = true;
	bool debug = false;

	SpriteRenderer* renderer;
	TextRenderer* textRenderer;
};

