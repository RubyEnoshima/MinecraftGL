#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>

struct Pla {
public:
	Pla(const glm::vec3& _pos, const glm::vec3& _normal) {
		normal = _normal;
		pos = _pos;
	}
	glm::vec3 normal = { 0, 0, 0 };
	glm::vec3 pos = { 0,0,0 };

	float distancia(const glm::vec3& p) const{
		return glm::dot(normal, (p - pos));
	}
};

using namespace std;
class Recursos
{
public:
	static enum TIPUS_MON {
		NORMAL = 0,
		PLA = 1
	};
	static glm::vec3* obtColor(string color) {
		return &COLORS[color];
	}
	typedef map<string, glm::vec3> MapColors;
	static MapColors COLORS;
	bool start = false;
private:
	Recursos() { 
	}

};

