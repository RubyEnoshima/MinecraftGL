#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>

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
	
private:
	Recursos() { 
	}

};

