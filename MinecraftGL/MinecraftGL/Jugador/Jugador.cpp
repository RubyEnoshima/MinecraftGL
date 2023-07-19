#include "Jugador.h"

Jugador::Jugador(Camera* _camera)
{
	camera = _camera;
	inventari = new Inventari();
}

Jugador::~Jugador()
{
	delete camera;
	delete inventari;
}

void Jugador::update(float deltaTime, const vector<pair<glm::vec3, uint8_t>>& blocs)
{
	
	if (mode == ESPECTADOR) {
		if (vel.x > 0) camera->moureDreta(deltaTime, vel.x);
		else if (vel.x < 0) camera->moureEsquerra(deltaTime, -vel.x);

		if (vel.z > 0) camera->moureDavant(deltaTime, vel.z);
		else if (vel.z < 0) camera->moureDarrera(deltaTime, -vel.z);

		return;
	}

	posSeg = obtPos() + vel;
	posSeg = glm::vec3(floor(posSeg.x), floor(posSeg.y) - ALTURA_JUG, floor(posSeg.z));
	if (colisiona(blocs)) {
		parar();
		vel.y = 0;
	}
	else {
		if (vel.x > 0) camera->moureDreta(deltaTime, vel.x);
		else if (vel.x < 0) camera->moureEsquerra(deltaTime, -vel.x);

		if (vel.z > 0) camera->moureDavant(deltaTime, vel.z);
		else if (vel.z < 0) camera->moureDarrera(deltaTime, -vel.z);
		
		vel.y -= GRAVETAT * deltaTime;
	}
	
	if (vel.y < 0) camera->moureAvall(deltaTime, -vel.y);
	else if (vel.y > 0) camera->moureAmunt(deltaTime, vel.y);

	velAnt = obtPos() - posAnt;
	posAnt = obtPos();
}

void Jugador::moure(float deltaTime, int tecla)
{
	switch (tecla)
	{
		case GLFW_KEY_W:
			vel.z = velocitatAct;
			break;
		case GLFW_KEY_A:
			vel.x = -velocitatAct;
			break;
		case GLFW_KEY_S:
			vel.z = -velocitatAct;
			break;
		case GLFW_KEY_D:
			vel.x = velocitatAct;
			break;
		case GLFW_KEY_SPACE: 
			if (mode == ESPECTADOR) camera->moureAmunt(deltaTime, velocitatAct);
			else if(vel.y == 0) vel.y = SALT;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			if (mode == ESPECTADOR) camera->moureAvall(deltaTime, velocitatAct);
			break;
	}
}

void Jugador::parar()
{
	vel.x = 0;
	vel.z = 0;
}

void Jugador::correr()
{
	velocitatAct = velocitat * 1.5f;
}

void Jugador::caminar()
{
	velocitatAct = velocitat;
}

bool Jugador::colisiona(const vector<pair<glm::vec3, uint8_t>>& blocs)
{
	//if (blocs == anteriors) return ultimResultat; // tecnicament false???
	//anteriors = blocs;
	//glm::vec3 pos = obtPosBloc();
	for (const auto &bloc : blocs)
	{
		if (colisiona(bloc.first)) return true;
	}
	//ultimResultat = false;
	return false;
}

bool Jugador::colisiona(const glm::vec3& bloc)
{
	if (bloc == obtPosBloc()) {
		return true; 
	}
	return false;
}

bool Jugador::sotaAigua(const vector<pair<glm::vec3, uint8_t>>& blocs) const
{
	for (const auto& bloc : blocs)
	{
		if (bloc.first == obtPosBloc(false) && bloc.second == AIGUA) {
			return true;
		}
	}
	return false;
}

glm::vec3 Jugador::obtPos() const
{
	return camera->obtPos();
}

glm::vec3 Jugador::obtPosBloc(bool peu) const
{
	glm::vec3 pos = obtPos();
	if(peu) return glm::vec3(floor(pos.x), floor(pos.y) - ALTURA_JUG, floor(pos.z));
	return glm::vec3(floor(pos.x), floor(pos.y), floor(pos.z));
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
	vel.y = 0;
}
