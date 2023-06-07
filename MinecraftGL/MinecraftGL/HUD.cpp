#include "HUD.h"

HUD::HUD(Renderer* _renderer)
{
	//glActiveTexture(GL_TEXTURE1);
	ShaderProgram* shader = new ShaderProgram("VertexSprite.vert", "FragmentSprite.frag");
	renderer = new SpriteRenderer(shader,_renderer);
	iniciaSprites();
}

HUD::~HUD()
{
	delete renderer;
}

void HUD::render()
{
	if(visible) renderer->render();
}

void HUD::alternaVisibilitat()
{
	visible = !visible;
}

void HUD::iniciaSprites()
{
	Sprite* sprite = new Sprite(new Textura("inventari.png"), "Inventari", glm::vec2(renderer->width / 2, renderer->height / 2), glm::vec2(7));
	sprite->centrat = true;
	sprite->pos = glm::vec2(renderer->width / 2, renderer->height - sprite->tamany.y*1.75);
	renderer->afegirSprite(sprite);

	Sprite* crosshair = new Sprite(new Textura("icons.png"), "Crosshair", glm::vec2(renderer->width / 2, renderer->height / 2), glm::vec2(0.4));
	crosshair->centrat = true;
	crosshair->tamanyMapa = glm::vec2(16, 16);
	crosshair->posicioMapa = glm::vec2(0, 0);
	crosshair->color.a = 0.85;
	crosshair->textura->mapa = true;
	renderer->afegirSprite(crosshair);
}
