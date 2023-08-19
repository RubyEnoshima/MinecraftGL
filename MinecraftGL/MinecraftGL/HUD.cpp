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

void HUD::render(const glm::vec3&posJug, const glm::vec3&posMira)
{
	if (visible) {
		renderer->render();
		if (debug) renderDebug(posJug,posMira);
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

void HUD::renderDebug(const glm::vec3& posJug, const glm::vec3& posMira)
{
	textRenderer->RenderText("MinecraftGL - Ruben Lopez", 5, 5, 0.2, true, { 1,1,1 }, true);
	textRenderer->RenderText("Jugador - X: " + to_string((int)posJug.x) + ", Y: " + to_string((int)posJug.y) + ", Z: " + to_string((int)posJug.z), 5, 35, 0.2, true, { 1,1,1 }, true);
	textRenderer->RenderText("Cub mira - X: "+to_string((int)posMira.x)+", Y: "+ to_string((int)posMira.y) +", Z: " + to_string((int)posMira.z), 5, 65, 0.2, true, {1,1,1}, true);
}
