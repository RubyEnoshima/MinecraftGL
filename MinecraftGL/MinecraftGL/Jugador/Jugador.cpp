#include "Jugador.h"

Jugador::Jugador(Camera* _camera, ShaderProgram* shader)
{
	camera = _camera;
	inventari = new Inventari();
	aabb.tamany = glm::vec3(1,1,1);
}

Jugador::~Jugador()
{
	delete camera;
	delete inventari;
}

void Jugador::update(float deltaTime, const vector<AABB>& blocs)
{
	if (mode != ESPECTADOR) {
		aabb.pos = obtPosBloc();
		aabb.vel = vel;
		_blocs = blocs;
		float nx, ny = 0, nz; bool sota = false;
		
		for (const auto& b : blocs)
		{
			float col = aabb.sweptAABB(b, nx, ny, nz);

			if (col < 1) {				
				if (nx != 0)vel.x = 0;
				if (nz != 0)vel.z = 0;
				
				// Colisió en la y: mirem la normal per aplicar o no gravetat
				if (ny != 0) {
					vel.y = 0;
					if (ny == -1) enTerra = true;

				}
			}
			if ((aabb.pos - b.pos).y == 1) { 
 				sota = true; 
			}
		}
		if (!sota) enTerra = false;
		if (!enTerra) {
			vel.y -= GRAVETAT * deltaTime;
		}

	}

	camera->teletransporta(obtPos() + vel * deltaTime);
}

void Jugador::moure(const map<int,bool>& tecles)
{
	glm::vec3 novaVelForw = velocitatAct * glm::normalize(glm::vec3(camera->obtDireccio().x, 0, camera->obtDireccio().z));
	glm::vec3 novaVelRight = glm::vec3(camera->obtRight().x * velocitatAct, 0, camera->obtRight().z * velocitatAct);
	vel.x = 0;
	vel.z = 0;
	for (auto tecla : tecles) {
		if (tecla.second) {
			switch (tecla.first)
			{
				case GLFW_KEY_W:
					vel.x += novaVelForw.x;
					vel.z += novaVelForw.z;
					break;
				case GLFW_KEY_A:
					vel.x += -novaVelRight.x;
					vel.z += -novaVelRight.z;
					break;
				case GLFW_KEY_S:
					vel.x += -novaVelForw.x;
					vel.z += -novaVelForw.z;
					break;
				case GLFW_KEY_D:
					vel.x += novaVelRight.x;
					vel.z += novaVelRight.z;
					break;
				case GLFW_KEY_SPACE:
					if (mode == ESPECTADOR) vel.y = velocitatAct;
					else if (enTerra) {
						vel.y = SALT; enTerra = false;
					}
					break;
				case GLFW_KEY_LEFT_SHIFT:
					if (mode == ESPECTADOR) vel.y = -velocitatAct;
					break;
			}
		}
	}
	
}

void Jugador::parar()
{
	vel.x = 0;
	vel.z = 0;
	if(mode == ESPECTADOR) vel.y = 0;
}

void Jugador::correr()
{
	velocitatAct = velocitat * 1.5f;
}

void Jugador::caminar()
{
	velocitatAct = velocitat;
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
	if (mode == _mode) return;
	mode = _mode;
	vel.y = 0;
	enTerra = false;
}