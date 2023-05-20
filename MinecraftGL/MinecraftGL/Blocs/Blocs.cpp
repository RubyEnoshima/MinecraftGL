#include "Blocs.h"

Blocs::Blocs() {
    // per tots els tipus de blocs que hi ha
    for (auto& p : fs::directory_iterator(fs::directory_iterator("./Tipus/"))) {
        cout << "Carregant blocs..." << endl;
        ifstream f(p.path());
        // Utilitzant json llegim tots els tipus de blocs que hi ha
        json data = json::parse(f);
        int id = data.value("id", 0);
        string nom = data.value("nom", "");
        Bloc bloc(nom, id, data.value("transparent",false), data.value("costats",id), data.value("sota",id), data.value("adalt",id));
        //lowercase(nom);
        dades.insert({ id, bloc});
    }
    cout << "Blocs carregats!" << endl << endl;
}