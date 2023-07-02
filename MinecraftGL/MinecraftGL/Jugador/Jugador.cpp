#include "Jugador.h"

Jugador::Jugador(Camera* _camera, int _mode)
{
	camera = _camera;
	mode = _mode;
}

Jugador::~Jugador()
{
	delete camera;
}

void Jugador::update(float deltaTime)
{
	if (mode == ESPECTADOR) return;

	vel.y += GRAVETAT * deltaTime;
	camera->moureAvall(deltaTime, vel.y);
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

bool Jugador::colisiona(const vector<glm::vec3>& blocs) 
{
	if (blocs == anteriors) return ultimResultat; // tecnicament false???
	anteriors = blocs;
	glm::vec3 pos = obtPosBloc();
	for (auto bloc : blocs)
	{
		if (bloc == pos) { ultimResultat = true; return true; }
	}
	ultimResultat = false;
	return false;
}

bool Jugador::colisiona(const glm::vec3& bloc) const
{
	return bloc == obtPosBloc();
}

glm::vec3 Jugador::obtPos() const
{
	return camera->obtPos();
}

glm::vec3 Jugador::obtPosBloc() const
{
	glm::vec3 pos = obtPos();
	return glm::vec3(floor(pos.x), floor(pos.y) - ALTURA_JUG, floor(pos.z));
}

glm::vec2 Jugador::obtPos2D() const
{
	return glm::vec2(camera->obtPos().x,camera->obtPos().z);
}

Camera* Jugador::obtCamera() const
{
	return camera;
}

glm::vec2 Jugador::chunkActual() const
{
	glm::vec3 pos = obtPosBloc();
	return glm::vec2((int)(pos.x/X),(int)(pos.z/Z));
}

void Jugador::canviaMode(int _mode)
{
	mode = _mode;
}
