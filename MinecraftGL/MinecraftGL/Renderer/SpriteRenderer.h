#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Shaders/ShaderProgram.h"
#include "Textura.h"
#include "glm/gtx/io.hpp"
#include <map>

// Es guarda tota la informació que pot tenir un Sprite
class Sprite {
public:
	Sprite(Textura* _textura, string _nom, const glm::vec2& _pos, const glm::vec2& _escala = glm::vec2(1.0f), float _rotacio = 0.0f) {
		pos = _pos;
		textura = _textura;
		escala = _escala;
		rotacio = _rotacio;
		tamany = _textura->obtTamany();
		nom = _nom;
	};
	~Sprite() {
		delete textura;
	}

	// Atributs per renderitzar
	glm::vec2 pos = glm::vec2(0.0f);
	glm::vec2 tamany = glm::vec2(0.0f);
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec2 escala = glm::vec2(1.0f);
	float rotacio = 0.0f;

	// Si fals, no es renderitzarà
	bool visible = true;
	// Si fals, es renderitzarà a partir de la cantonada esquerra superior. Si true, es renderitzarà a partir del centre del sprite.
	bool centrat = false; 
	string nom = "";

	Textura* textura;
};

class SpriteRenderer
{
public:
	SpriteRenderer(ShaderProgram* _shader, Renderer* _renderer);
	~SpriteRenderer();

	// Renderitza tots els sprites afegits que siguin visibles
	void render();
	// Afegeix un sprite per tal que es pugui renderitzar. Si ja hi havia un amb el mateix nom, el sobreescriu
	void afegirSprite(Sprite* sprite);
	// Retorna un punter a un sprite
	Sprite* obtSprite(string nom) const;
	
	float width, height;

private:
	// Inicia els vertex que pot tenir un sprite (sempre seran un quadrat)
	void initRenderData();

	// Renderitza un sprite
	void DrawSprite(Sprite* sprite);

	// Donada una posició en píxels [0-width][0-height], retorna la posició en coordenades pantalla [-1,1][-1,1]
	glm::vec2 normalitzarPos(const glm::vec2& pos, int w, int h);

	map<string,Sprite*> Sprites;
	Renderer* renderer;
	ShaderProgram* shader;
	unsigned int quadVAO;
};

