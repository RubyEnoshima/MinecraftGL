#pragma once
#include "Renderer/SpriteRenderer.h"
class HUD
{
public:
	HUD(Renderer* _renderer);
	~HUD();

	void render();
private:
	Textura inventari;

	SpriteRenderer* renderer;
};

