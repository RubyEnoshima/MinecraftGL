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
	Textura(string _nom);
	void use();
	glm::vec2 obtTamany() const;
	unsigned int textura;
private:
	string nom;
	unsigned char* data;
	int width, height, nrChannels;
};

