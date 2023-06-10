#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
using namespace std;
class Recursos
{
public:
	static glm::vec3* obtColor(string color) {
		return &COLORS[color];
	}
	typedef map<string, glm::vec3> MapColors;
	static MapColors COLORS;

	static glm::vec3 hexToRGB(string hex) {
		string redHex = hex.substr(1, 2);
		string greenHex = hex.substr(3, 2);
		string blueHex = hex.substr(5, 2);

		int red = stoi(redHex, nullptr, 16);
		int green = stoi(greenHex, nullptr, 16);
		int blue = stoi(blueHex, nullptr, 16);

		return glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
	}
private:
	Recursos() { 
	}

};

