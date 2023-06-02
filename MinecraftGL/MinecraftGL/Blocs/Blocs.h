#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

struct Bloc {
    Bloc(string _nom,int _id,bool _trans,int _cost,int _sota,int _adalt,bool _vegetal){
        nom = _nom;
        id = _id;
        transparent = _trans;
        costats = _cost;
        sota = _sota;
        adalt = _adalt;
        vegetacio = _vegetal;
    }
    Bloc() {
        nom = "";
        id = 0;
        transparent = false; 
        costats = 0; 
        sota = 0; 
        adalt = 0;
        vegetacio = false;
    }
    
    string nom;
    int id;
    bool transparent; // Per temes de llum i renderitzat
    bool vegetacio; // Per saber com s'ha de renderitzar
    int costats; // Textura que s'ha d'utiltizar als costats del cub
    int sota; // Textura per sota del cub
    int adalt;
};

class Blocs {
public:
    Blocs();
    ~Blocs();
    Bloc* getBloc(int id) const{
        return dades.at(id);
    }
    
private:
    vector<Bloc*> dades;
};