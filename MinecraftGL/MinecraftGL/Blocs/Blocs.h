#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

struct Bloc {
    Bloc(string _nom,int _id,bool _trans,int _cost,int _sota,int _adalt){
        nom = _nom;
        id = _id;
        transparent = _trans;
        costats = _cost;
        sota = _sota;
        adalt = _adalt;
    }
    Bloc() {
        nom = "";
        id = 0;
        transparent = false; // Per temes de llum i renderitzat
        costats = 0; // Textura que s'ha d'utiltizar als costats del cub
        sota = 0; // Textura per sota del cub
        adalt = 0;
    }
    string nom;
    int id;
    bool transparent; // Per temes de llum i renderitzat
    int costats; // Textura que s'ha d'utiltizar als costats del cub
    int sota; // Textura per sota del cub
    int adalt;
};

class Blocs {
public:
    static Blocs& get() {
        static Blocs instance;
        return instance;
    }

    Bloc getBloc(int id) const{
        return dades.at(id);
    }
    
private:
    Blocs();
    ~Blocs() = default;

    vector<Bloc> dades;
};