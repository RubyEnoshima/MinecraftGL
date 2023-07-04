#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/gtx/io.hpp"
#include "Textura.h"
#include "Recursos.h"

using namespace std;
class Sprite {
public:
	Sprite(Textura* _textura, string _nom, const glm::vec2& _pos, const glm::vec2& _escala = glm::vec2(1.0f), bool _centrat = false) {
		pos = _pos;
		textura = _textura;
		Recursos::afegirTextura(_textura);
		escala = _escala;
		tamany = tamanyMapa = _textura->obtTamany();
		nom = _nom;
		centrat = _centrat;
		transformar();
	};
	~Sprite() {
		//		delete textura;
	}

	// Funcions per transformar l'sprite. Faran que el seu model sigui modificat
	void teletransportar(const glm::vec2& novaPos);
	void moure(const glm::vec2& offset);
	void escalar(const glm::vec2& novaEscala);
	void centrar(bool centrar = true); 

	glm::vec2 obtPos() const;
	glm::vec2 obtTamany() const;

	// Si fals, no es renderitzar�
	bool visible = true;

	// Si la textura �s un mapa, es pot posar quina posici� es vol renderitzar
	glm::vec2 posicioMapa = glm::vec2(0.0f); 
	// Si la textura �s un mapa, es pot posar quants p�xels ocupa l'sprite
	glm::vec2 tamanyMapa = glm::vec2(1.0f); 

	glm::vec4 color = glm::vec4(1.0f);
	
	// Transformaci� final de l'sprite
	glm::mat4 model;

	Textura* textura;
	string nom = "";
	float indexZ = 0.0f;
private:
	void transformar();

	// Atributs per renderitzar
	glm::vec2 pos = glm::vec2(0.0f);
	glm::vec2 tamany = glm::vec2(0.0f);
	glm::vec2 escala = glm::vec2(1.0f);

	float rotacio = 0.0f;

	// Si fals, es renderitzar� a partir de la cantonada esquerra superior. Si true, es renderitzar� a partir del centre del sprite.
	bool centrat = false;
	
	// Donada una posici� en p�xels [0-width][0-height], retorna la posici� en coordenades pantalla [-1,1][-1,1]
	static glm::vec2 normalitzarPos(const glm::vec2& pos, int w, int h);
	
};