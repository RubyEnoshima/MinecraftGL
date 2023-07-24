#include "Blocs.h"

Blocs::Blocs() {
    
    dades.assign(MAX_BLOCS,NULL);
    for (int i = 0; i < MAX_BLOCS; i++) {
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
        glm::vec3 color = glm::vec3(1.0f);
        dades[id] = new Bloc{ nom, id, data.value("transparent", false), data.value("semitransparent", false), data.value("solid", true), data.value("vegetacio",false), data.value("costats", id), data.value("sota", id), data.value("adalt", id)};
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
