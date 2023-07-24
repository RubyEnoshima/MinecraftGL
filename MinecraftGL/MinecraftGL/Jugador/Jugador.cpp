#include "Jugador.h"

Jugador::Jugador(Camera* _camera, ShaderProgram* shader)
{
	camera = _camera;
	inventari = new Inventari();
	aabb.tamany = glm::vec3(1,2,1);
	aabb.shader = shader;
}

Jugador::~Jugador()
{
	delete camera;
	delete inventari;
}

void Jugador::update(float deltaTime, const vector<AABB>& blocs)
{
	aabb.pos = obtPosBloc();
	aabb.vel = vel;
	_blocs = blocs;
	float nx, ny = 0, nz; bool sota = false;
	//cout << vel << endl;
	for (const auto& b : _blocs)
	{
		//if (aabb.AABBtoAABB(b)) {
		//	cout << aabb.pos - b.pos << endl;
		//	vel = glm::vec3(0);
		//	camera->teletransporta(antPos);
		//	//if ((aabb.pos - b.pos).y == 1) enTerra = true;
		//}
		
		float col = aabb.sweptAABB(b, nx, ny, nz);
		float rem = 1 - col;
		if (col < 1) {
			//cout << nx << " " << ny << " " << nz << endl;

			float dotprod = (aabb.vel.x * nz + aabb.vel.z * nx) * rem;
			vel.x = dotprod * nz; vel.z = dotprod * nx;
			/*vel.x *= col;
			vel.z *= col;*/

			// Colisi� en la y: mirem la normal per aplicar o no gravetat
			if (ny != 0) {
				vel.y = 0;
				if (ny == -1) enTerra = true;

			}
		}
		if ((aabb.pos - b.pos).y == 1) { sota = true; }
	}
	if (!sota) enTerra = false;
	if (mode != ESPECTADOR && !enTerra) {
		vel.y -= GRAVETAT * deltaTime;
	}

	antPos = obtPos();
	camera->teletransporta(obtPos() + vel * deltaTime);
}

void Jugador::render()
{
	//aabb.render();
	/*for (const auto& b : _blocs)
	{
		b.render();
	}*/
}

void Jugador::moure(float deltaTime, const map<int,bool>& tecles)
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
	if (mode == _mode) return;
	mode = _mode;
	vel.y = 0;
	enTerra = false;
}

void Jugador::actualitzaCamera(float deltaTime)
{
	if (vel.x > 0) camera->moureDreta(deltaTime, vel.x);
	else if (vel.x < 0) camera->moureEsquerra(deltaTime, -vel.x);

	if (vel.z > 0) camera->moureDavant(deltaTime, vel.z);
	else if (vel.z < 0) camera->moureDarrera(deltaTime, -vel.z);

	if (vel.y < 0) camera->moureAvall(deltaTime, -vel.y);
	else if (vel.y > 0) camera->moureAmunt(deltaTime, vel.y);
}
