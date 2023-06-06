#include "HUD.h"

HUD::HUD(Renderer* _renderer)
{
	glActiveTexture(GL_TEXTURE1);
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
	renderer->render();
}

void HUD::iniciaSprites()
{
	Sprite* sprite = new Sprite(new Textura("inventari.png"), "Inventari", glm::vec2(renderer->width / 2, renderer->height / 2), glm::vec2(2));
	sprite->mode = true;
	sprite->pos = glm::vec2(renderer->width / 2, renderer->height - sprite->tamany.y);
	renderer->afegirSprite(sprite);
	
	Sprite* uyuyuy = new Sprite(new Textura("easteregg.png"),"UwU", glm::vec2(renderer->width / 2, renderer->height / 2), glm::vec2(1));
	uyuyuy->mode = true;
	uyuyuy->visible = false;

	renderer->afegirSprite(uyuyuy);

}
