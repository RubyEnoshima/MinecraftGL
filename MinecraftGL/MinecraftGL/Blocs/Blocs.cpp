#include "Blocs.h"

Blocs::Blocs() {
    dades = vector<Bloc>(256);

    ifstream arxiuBlocs("./Tipus/blocs.json"); // arxiu json amb tots els tipus de blocs
    json jsonBlocs = json::parse(arxiuBlocs);

    auto it = jsonBlocs.begin();
    cout << "Carregant blocs..." << endl;

    // Per tots els blocs que hi hagi a l'arxiu
    while (it != jsonBlocs.end()) {
        json data = *it;
        int id = data.value("id", 0);
        string nom = data.value("nom", "");
        dades[id] = Bloc(nom, id, data.value("transparent", false), data.value("costats", id), data.value("sota", id), data.value("adalt", id));
        it++;
    }

    cout << "Blocs carregats!" << endl << endl;
}