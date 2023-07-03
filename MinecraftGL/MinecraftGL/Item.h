#pragma once
#include "Renderer/SpriteRenderer.h"
class Item
{
public:
	void render();
private:
	int id;
	bool esBloc;

	Sprite sprite;
};

