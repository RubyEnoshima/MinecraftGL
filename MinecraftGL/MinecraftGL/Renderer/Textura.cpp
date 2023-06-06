#include "Textura.h"

Textura::Textura()
{
	nom = "";
	data = nullptr;
	width = height = nrChannels = textura = 0;
}

Textura::Textura(string _nom)
{
	nom = "Textures/"+_nom;

	glGenTextures(1,&textura);
	glBindTexture(GL_TEXTURE_2D, textura);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load(nom.c_str(), &width, &height, &nrChannels, 4);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		cout << "ERROR!!! No s'ha pogut carregar la textura " << _nom << endl;
	}
		// pagina 60
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Textura::use()
{
	glBindTexture(GL_TEXTURE_2D, textura);
}

glm::vec2 Textura::obtTamany() const
{
	return glm::vec2(width, height);
}
