#include "Joc.h"

int main() {
	cout << "MinecraftGL - Ruben Lopez" << endl;

	Joc joc;

	if (joc.crearFinestra()) {
		joc.start();
	}

	return 0;
}