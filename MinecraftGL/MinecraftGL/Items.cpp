#include "Items.h"

Items::Items()
{
    dades.assign(MAX_ITEMS, NULL);
    for (int i = 0; i < MAX_ITEMS; i++) {
        dades[i] = new Item();
    }

    ifstream arxiuItems("./Tipus/items.json"); // arxiu json amb tots els tipus de items
    json jsonItems = json::parse(arxiuItems);

    auto it = jsonItems.begin();
    cout << "Carregant items..." << endl;

    // Per tots els items que hi hagi a l'arxiu
    while (it != jsonItems.end()) {
        json data = *it;
        int id = data.value("id", -1);
        string nom = data.value("nom", "");
        dades[id] = new Item{ id, data.value("classe", 0), nom, (uint8_t)data.value("sprite", 0), (uint8_t)data.value("bloc_id", -1) };
        it++;
    }

    cout << "Items carregats!" << endl << endl;
}

Items::~Items()
{
    for (int i = 0; i < dades.size(); i++) {
        delete dades[i];
    }
}
