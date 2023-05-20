#pragma once
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <json.hpp>
using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

struct Bloc {
    Bloc(string _nom,int _id,bool _trans,int _cost,int _sota,int _adalt){
        nom = _nom;
        id = _id;
        transparent = _trans;
        costats = _cost;
        sota = _sota;
        adalt = _adalt;
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

    string nom(int id) {
        auto it = dades.find(id);
        if (it != dades.end()) {
            return (*it).second.nom;
        }
        return ""; // Per si encara hi ha blocs que no he implementat en json
    }

    bool esTransparent(int id) {
        auto it = dades.find(id);
        if (it != dades.end()) {
            return (*it).second.transparent;
        }
        return false;
    }

    int costats(int id) {
        auto it = dades.find(id);
        if (it != dades.end()) {
            return (*it).second.costats;
        }
        return id;
    }

    int sota(int id) {
        auto it = dades.find(id);
        if (it != dades.end()) {
            return (*it).second.sota;
        }
        return id;
    }

    int adalt(int id) {
        auto it = dades.find(id);
        if (it != dades.end()) {
            return (*it).second.adalt;
        }
        return id;
    }
    
private:
    Blocs();
    ~Blocs() = default;

    void lowercase(string& str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    map<int, Bloc> dades;
};