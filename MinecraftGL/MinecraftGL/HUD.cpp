#include "HUD.h"

HUD::HUD(Renderer* _renderer, Inventari* _inventari)
{
	//glActiveTexture(GL_TEXTURE1);
	ShaderProgram* shader = new ShaderProgram("VertexSprite.vert", "FragmentSprite.frag");
	renderer = new SpriteRenderer(shader,_renderer);
	iniciaSprites();
	textRenderer = new TextRenderer(Recursos::width,Recursos::height);
	textRenderer->Load("Fonts/Minecraft.otf", 72*2);
	_inventari->iniciaSprites(renderer,textRenderer);
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
		if (debug) renderDebug();
	}
}

void HUD::alternaVisibilitat()
{
	visible = !visible;
}

void HUD::modeDebug()
{
	debug = !debug;
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

void HUD::renderDebug()
{
	textRenderer->RenderText("MinecraftGL - Ruben Lopez", 5, 5, 0.25, true, {1,1,1}, true);
}
