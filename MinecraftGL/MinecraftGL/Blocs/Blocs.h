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
    Bloc(string _nom,int _id,bool _trans,bool _semi,int _cost,int _sota,int _adalt,bool _vegetal,const glm::vec3& _color){
        nom = _nom;
        id = _id;
        transparent = _trans;
        semitransparent = _semi;
        costats = _cost;
        sota = _sota;
        adalt = _adalt;
        vegetacio = _vegetal;
        color = _color;
    }
    Bloc() {
        nom = "";
        id = 0;
        transparent = false; 
        semitransparent = false;
        costats = 0; 
        sota = 0; 
        adalt = 0;
        vegetacio = false;
        color = glm::vec3(1.0f);
    }
    
    string nom;
    int id;
    bool transparent; // Per temes de llum i renderitzat
    bool semitransparent; // Per temes de llum i renderitzat
    bool vegetacio; // Per saber com s'ha de renderitzar
    int costats; // Textura que s'ha d'utiltizar als costats del cub
    int sota; // Textura per sota del cub
    int adalt;
    glm::vec3 color;
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