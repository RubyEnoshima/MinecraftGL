#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	fbo = 0;
	rbo = 0;
	textura = 0;
}

Framebuffer::~Framebuffer()
{
	//glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &textura);
}

Framebuffer::Framebuffer(int width, int height) {
	// Generar framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Creem la textura pero encara no posem res!
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Fan falta...?
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Li posem la textura
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textura, 0);

	// Fem un renderbuffer pel depth (fa falta...?)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);

	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Comprovem errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Hi ha hagut un problema amb el framebuffer." << endl;
		return;
	}
	
	// Fem que torni a la finestra
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::Unir() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::Desunir() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::ObtID() const
{
	return fbo;
}

unsigned int Framebuffer::ObtTextura() const
{
	return textura;
}
