#include "Generador.h"

Generador::Generador()
{
	if (LLAVOR_RANDOM) {
		llavor = time(NULL);
	}
	srand(llavor);

	// Aquí podem definir diferents tipus de sorolls i la seva importància.

	Soroll continentalness;
	continentalness.noise = new FastNoiseLite(llavor);
	continentalness.noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	continentalness.noise->SetFrequency(0.0075);
	continentalness.noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	continentalness.noise->SetFractalLacunarity(2);
	continentalness.noise->SetFractalGain(0.3);
	continentalness.noise->SetFractalOctaves(7);
	continentalness.importancia = 0.75;
	continentalness.punts = { glm::vec2(-1,0), glm::vec2(0,70), glm::vec2(0.5,95), glm::vec2(0.7,105), glm::vec2(1,110) };

	noises.push_back(continentalness);

	Soroll erosion;
	erosion.noise = new FastNoiseLite(llavor);
	erosion.noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	erosion.noise->SetFrequency(0.005);
	erosion.noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	erosion.noise->SetFractalLacunarity(0.05);
	erosion.noise->SetFractalGain(0.8);
	erosion.noise->SetFractalOctaves(3);
	erosion.importancia = 0.5;
	erosion.punts = { glm::vec2(-1,0), glm::vec2(0,15), glm::vec2(0.75,20), glm::vec2(1,20) };
	noises.push_back(erosion);

	//Soroll pv;
	//pv.noise = new FastNoiseLite(semilla);
	//pv.noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	//pv.noise->SetFrequency(0.018);
	//pv.noise->SetFractalType(FastNoiseLite::FractalType_Ridged);
	//pv.noise->SetFractalLacunarity(1);
	//pv.noise->SetFractalGain(0.2);
	//pv.noise->SetFractalOctaves(2);
	//pv.importancia = 0.1;
	//pv.punts = { glm::vec2(-1,5), glm::vec2(-0.75,20), glm::vec2(-0.25,30), glm::vec2(0,35), glm::vec2(0.5,100), glm::vec2(1,100) };
	////pv.punts = { glm::vec2(-1,0), glm::vec2(-0.75,50), glm::vec2(0.5,40), glm::vec2(1,20) };
	//noises.push_back(pv);
}

Generador::~Generador()
{
	for (auto noise : noises) delete noise.noise;

}

int Generador::obtFlor() const
{
	return flors[rand() % flors.size()];
}

int Generador::obtTipus(int altura, int max) const
{
	uint8_t tipus = nivells[2]; // Capa mitja-alta
	if (altura <= nivellMar && altura > max - 3) tipus = nivells[3]; // Platjes
	else if (altura == max) { // Capa d'adalt
		if (max >= nivellNeu) tipus = nivells[5];
		else tipus = nivells[4];
	}
	else if (altura < max - 3) tipus = nivells[1]; // Capa mitja
	else if (altura < 1) tipus = nivells[0]; // Capa d'abaix
	return tipus;
}

int Generador::obtAltura(float x, float y) const
{
	int i = 0, res = 0;
	for (const Soroll& noise : noises)
	{
		double soroll = noise.noise->GetNoise(x, y);
		res += interpolarSegments(noise.punts, soroll) * noise.importancia;
		//if (res <= 60) res *= .99;
		i++;
	}
	return res;
}
