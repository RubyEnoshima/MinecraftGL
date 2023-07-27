#pragma once
#include "Renderer.h"
#include "Shaders/ShaderProgram.h"
#include "Sprite.h"

#include <map>

class SpriteRenderer
{
public:
	SpriteRenderer(ShaderProgram* _shader, Renderer* _renderer);
	~SpriteRenderer();

	// Renderitza tots els sprites afegits que siguin visibles
	void render();
	// Afegeix un sprite per tal que es pugui renderitzar. Si ja hi havia un amb el mateix nom, no s'afegfirà!
	void afegirSprite(Sprite* sprite);
	// Elimina un sprite
	void eliminaSprite(string nom);
	// Retorna un punter a un sprite
	Sprite* obtSprite(string nom) const;
	// Canvia el zIndex d'un sprite
	void canviaIndex(string nom, int nouIndex);
	
	// Renderitza un sprite
	void DrawSprite(Sprite* sprite);
	float width, height;

	ShaderProgram* shader;
private:
	// Inicia els vertex que pot tenir un sprite (sempre seran un quadrat)
	void initRenderData();


	

	multimap<int,Sprite*> SpritesOrdenats;
	map<string,Sprite*> Sprites;
	Renderer* renderer;
	unsigned int quadVAO, VBO;
};

