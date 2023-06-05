#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
using namespace std;
class Textura
{
public:
	Textura();
	Textura(string _nom);
	void use();

	unsigned int textura;
private:
	string nom;
	unsigned char* data;
	int width, height, nrChannels;
};

