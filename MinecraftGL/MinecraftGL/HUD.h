#pragma once
#include "Renderer/SpriteRenderer.h"
class HUD
{
public:
	HUD(Renderer* _renderer);
	~HUD();

	void render();
	void alternaVisibilitat();
private:
	void iniciaSprites();

	bool visible = true;

	Textura inventari;

	SpriteRenderer* renderer;
};

