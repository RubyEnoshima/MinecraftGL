#include "Textura.h"

Textura::Textura(string _nom)
{
	nom = _nom;
	data = stbi_load(_nom.c_str(), &width, &height, &nrChannels, 0);
	if(data)
	else
		// pagina 60
	glGenTextures(1,&textura);
	
}

void Textura::use()
{
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

}
