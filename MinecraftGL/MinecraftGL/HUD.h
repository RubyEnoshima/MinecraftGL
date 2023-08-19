#pragma once
#include "Renderer/SpriteRenderer.h"
#include "Renderer/TextRenderer.h"
#include "Inventari.h"
#include "glm/gtx/string_cast.hpp"
class HUD
{
public:
	HUD(Renderer* _renderer, Inventari* _inventari);
	~HUD();

	void render(const glm::vec3& posJug, const glm::vec3& posMira);

	void alternaVisibilitat();
	void modeDebug();
private:
	void iniciaSprites();
	void renderDebug(const glm::vec3& posJug, const glm::vec3& posMira);

	bool visible = true;
	bool debug = false;

	SpriteRenderer* renderer;
	TextRenderer* textRenderer;
};

