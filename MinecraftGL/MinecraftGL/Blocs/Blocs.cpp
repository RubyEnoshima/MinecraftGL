#include "Blocs.h"

Blocs::Blocs() {
    
    dades.assign(256,NULL);
    for (int i = 0; i < 256; i++) {
        dades[i] = new Bloc();
    }

    ifstream arxiuBlocs("./Tipus/blocs.json"); // arxiu json amb tots els tipus de blocs
    json jsonBlocs = json::parse(arxiuBlocs);

    auto it = jsonBlocs.begin();
    cout << "Carregant blocs..." << endl;

    // Per tots els blocs que hi hagi a l'arxiu
    while (it != jsonBlocs.end()) {
        json data = *it;
        int id = data.value("id", 0);
        string nom = data.value("nom", "");
        dades[id] = new Bloc(nom, id, data.value("transparent", false), data.value("costats", id), data.value("sota", id), data.value("adalt", id));
        it++;
    }

    cout << "Blocs carregats!" << endl << endl;
}

Blocs::~Blocs()
{
    for (int i = 0; i < dades.size(); i++) {
        delete dades[i];
    }
}
