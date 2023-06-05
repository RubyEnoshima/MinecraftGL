#include "HUD.h"

HUD::HUD(Renderer* _renderer)
{
	glActiveTexture(GL_TEXTURE1);
	inventari = Textura("inventari.png");
	ShaderProgram* shader = new ShaderProgram("VertexSprite.vert", "FragmentSprite.frag");
	renderer = new SpriteRenderer(shader,_renderer);

}

HUD::~HUD()
{
	delete renderer;
}

void HUD::render()
{
	renderer->DrawSprite(inventari, glm::vec2(-0.47, -1), glm::vec2(0.8f,0.17f));
}
