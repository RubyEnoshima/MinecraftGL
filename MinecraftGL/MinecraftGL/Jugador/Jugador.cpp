#include "Jugador.h"

Jugador::Jugador(Camera* _camera)
{
	camera = _camera;
}

Jugador::~Jugador()
{
	delete camera;
}

void Jugador::moure(float deltaTime, int tecla)
{
	switch (tecla)
	{
		case GLFW_KEY_W:
			camera->moureDavant(deltaTime, velocitatAct);
			break;
		case GLFW_KEY_A:
			camera->moureEsquerra(deltaTime, velocitatAct);
			break;
		case GLFW_KEY_S:
			camera->moureDarrera(deltaTime, velocitatAct);
			break;
		case GLFW_KEY_D:
			camera->moureDreta(deltaTime, velocitatAct);
			break;
		case GLFW_KEY_SPACE: 
			if (mode == ESPECTADOR) camera->moureAmunt(deltaTime, velocitatAct);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			if (mode == ESPECTADOR) camera->moureAvall(deltaTime, velocitatAct);
			break;
	}
}

void Jugador::correr()
{
	velocitatAct = velocitat * 1.5f;
}

void Jugador::caminar()
{
	velocitatAct = velocitat;
}

Camera* Jugador::obtCamera() const
{
	return camera;
}

glm::vec2 Jugador::chunkActual() const
{
	glm::vec3 pos = camera->obtPos();
	return glm::vec2((int)(pos.x/X),(int)(pos.z/Z));
}
