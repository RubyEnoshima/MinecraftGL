#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "stb_image.h"
using namespace std;
class Textura
{
public:
	Textura();
	~Textura();
	Textura(string _nom);

	void use();
	glm::vec2 obtTamany() const;

	bool mapa = false;
	string nom;
private:
	unsigned char* data;
	unsigned int textura;

	int width, height, nrChannels;

};

