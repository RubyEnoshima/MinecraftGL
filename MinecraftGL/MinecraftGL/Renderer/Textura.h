#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
using namespace std;
class Textura
{
public:
	Textura(string _nom);

	void use();

private:
	string nom;
	unsigned int textura;
	unsigned char* data;
	int width, height, nrChannels;
};

