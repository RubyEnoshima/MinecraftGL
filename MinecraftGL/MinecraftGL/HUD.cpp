#include "HUD.h"

HUD::HUD(Renderer* _renderer, Inventari* _inventari)
{
	//glActiveTexture(GL_TEXTURE1);
	ShaderProgram* shader = new ShaderProgram("VertexSprite.vert", "FragmentSprite.frag");
	renderer = new SpriteRenderer(shader,_renderer);
	iniciaSprites();
	_inventari->iniciaSprites(renderer);
	textRenderer = new TextRenderer(Recursos::width,Recursos::height);
	textRenderer->Load("Fonts/OCRAEXT.TTF", 24);
}

HUD::~HUD()
{
	delete renderer;
	delete textRenderer;
}

void HUD::render()
{
	if (visible) {
		renderer->render();
		textRenderer->RenderText("Hola hola hola", 1.0f, 1.0f, 1.0f);
	}
}

void HUD::alternaVisibilitat()
{
	visible = !visible;
}

void HUD::iniciaSprites()
{
	Sprite* crosshair = new Sprite(Recursos::obtTextura("icons.png"), "Crosshair", glm::vec2(renderer->width / 2, renderer->height / 2), glm::vec2(0.4), true);
	crosshair->tamanyMapa = glm::vec2(16, 16);
	crosshair->posicioMapa = glm::vec2(0, 0);
	crosshair->color.a = 0.85;
	crosshair->textura->mapa = true;
	renderer->afegirSprite(crosshair);
}
