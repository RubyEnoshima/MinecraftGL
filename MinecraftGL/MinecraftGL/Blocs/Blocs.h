#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <glm/glm.hpp>

using json = nlohmann::json;
using namespace std;

#define MAX_BLOCS 256

struct Bloc {
    string nom = "";
    int id = 0;
    bool transparent = false; // Per temes de llum i renderitzat
    bool semitransparent = false; // Per temes de llum i renderitzat
    bool solid = true; // Si es atravessable pel jugador
    bool vegetacio = false; // Per saber com s'ha de renderitzar
    int costats = 0; // Textura que s'ha d'utiltizar als costats del cub
    int sota = 0; // Textura per sota del cub
    int adalt = 0;
    //glm::vec3 color;
};

class Blocs {
public:
    Blocs();
    ~Blocs();
    Bloc* getBloc(int id) const{
        if (id == -1) return NULL;
        return dades.at(id);
    }
    
private:
    vector<Bloc*> dades;
};