#include "Joc.h"

int main() {
	Joc joc;

	if (joc.crearFinestra()) {
		joc.gameLoop();
	}

	return 0;
}