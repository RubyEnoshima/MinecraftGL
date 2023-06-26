#include "SuperChunk.h"

SuperChunk::SuperChunk()
{
	renderer = nullptr;
	/*for (int x = 0; x < SIZE; x++)
		for (int y = 0; y < SIZE; y++)
			Chunks[glm::vec2(x, y)] = nullptr;*/
	//generarChunk(glm::vec2(i, j), arbrets);
}

SuperChunk::~SuperChunk() {
	for (auto chunk : Chunks)
	{
		delete chunk.second;
	}
	glDeleteBuffers(1, &VAO);
}

SuperChunk::SuperChunk(Renderer* _renderer)
{
		srand(semilla);

	renderer = _renderer;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/*for (int i = 0; i < XC; i++)
	{
		for (int j = 0; j < YC; j++)
		{
			Chunks[i][j] = new Chunk(i,j,&blocs);
		}
	}*/
	/*for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			Chunks[glm::vec2(i,j)] = new Chunk(i, j, &blocs);
		}
	}*/
	vector<glm::vec3> arbrets;

	/*for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			generarChunk(glm::vec2(i,j),arbrets);
		}
	}*/

	/*for (int i = -DISTANCIA; i <= DISTANCIA; i++) {
		for (int j = -DISTANCIA; j <= DISTANCIA; j++) {
			if (abs(i) + abs(j) > DISTANCIA) continue;
			generarChunk(glm::vec2(i, j), arbrets);
		}
	}

	for (int i = 0; i < arbrets.size(); i++) {
		glm::vec3 pos = arbrets[i];
		arbre(pos.x,pos.y,pos.z);
	}

	for (auto chunk : Chunks) {
		calculaLlumNatural(chunk.first);
	}*/

	/*for (int i = 0; i < X * SIZE; i++) {
		for (int k = 0; k < Z * SIZE; k++) {
			calculaLlumNatural(i,k);

		}
	}*/

	carregat = true;
}

void SuperChunk::comprovarChunks(const glm::vec2& chunkJugador)
{
	//cout << "Jugador: " << chunkJugador;
	vector<glm::vec2> descarregar;
	for (auto chunk : Chunks) {
		//std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);

		if (chunk.second) {
			if (chunk.second->descarregant) continue;
			int distanciaX = abs(chunkJugador.x - chunk.first.x);
			int distanciaY = abs(chunkJugador.y - chunk.first.y);
			//cout << "\t" << chunk.first << ": " << distanciaX << " " << distanciaY;
			if (distanciaX + distanciaY > DISTANCIA) {
				//cout << " {DESCARTAT}";
				chunksDescarregar.push_back(chunk.second);
				descarregar.push_back(chunk.first);
				chunk.second->descarregant = true;
			}

		}
		//cout << endl;
	}

	for (auto chunk : descarregar) {
		std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);

		Chunks.erase(chunk);

	}

	//if (chunksDescarregar.size() > 0) {
		// MIRAR AL VOLTANT DEL JUGADOR Y POSAR EN CUA CHUNKS NOUS
		for (int i = -DISTANCIA; i <= DISTANCIA; i++) {
			for (int j = -DISTANCIA; j <= DISTANCIA; j++) {
				if (abs(i) + abs(j) > DISTANCIA) continue;
				glm::vec2 nouChunk = chunkJugador + glm::vec2(i,j);
				if (!esCarregat(nouChunk) && !existeixCua(chunksCarregar, nouChunk))chunksCarregar.push_back(nouChunk);
				
			}
		}
	//}
	//cout << "--------" << endl;

}

void SuperChunk::descarregarChunks()
{
	if (!chunksDescarregar.empty()) {
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < NCHUNKS; i++) {
			if (chunksDescarregar.empty()) break;
			//std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);
			//loadedChunksMutex.lock();
			std::lock_guard<std::recursive_mutex> lock(cuaMutex);
			Chunk* c = chunksDescarregar.back();
			chunksDescarregar.pop_back();
			//cout << "Descarregant chunk " << pos << endl;
			delete c;
			/*Chunks.erase(pos);*/
			//loadedChunksMutex.unlock();
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		//cout << "En eliminar " << NCHUNKS << " chunks he trigat " << duration.count() << endl;
	}

}

void SuperChunk::carregarChunks()
{
	if (!chunksCarregar.empty() && potGenerar) {
		auto start = std::chrono::high_resolution_clock::now();
		//std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);
		for (int i = 0; i < NCHUNKS; i++) {
			if (chunksCarregar.empty()) break;
			std::lock_guard<std::recursive_mutex> lock(cuaMutex);
			glm::vec2 pos = chunksCarregar.back();
			chunksCarregar.pop_back();
			if (!esCarregat(pos)) {
				//cout << "Carregant chunk " << pos << endl;
				vector<glm::vec3> arbrets;
				generarChunk(pos, arbrets);
				/*for (int i = 0; i < arbrets.size(); i++) {
					glm::vec3 posArbre = arbrets[i];
					arbre(posArbre.x, posArbre.y, posArbre.z);
				}*/

				

				//potGenerar = false;
			}

		}
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		//cout << "En crear " << NCHUNKS << " chunks he trigat " << duration.count() << endl;
	}
}

void SuperChunk::eliminaCarregats()
{
	for (int i = 0; i < NCHUNKS; i++) {
		if (ChunksReady.empty()) break;
		std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);

		Chunks[ChunksReady.back().first] = ChunksReady.back().second;
		ChunksReady.back().second->canviat = true;
		ChunksReady.pop_back();
	}
}

void SuperChunk::generarChunk(const glm::vec2& pos, vector<glm::vec3>&arbrets)
{
	Chunk* nou = new Chunk(pos.x, pos.y, &blocs);
	Chunk* up = NULL;
	Chunk* left = NULL;
	Chunk* right = NULL; 
	Chunk* down = NULL;
	if (esCarregat(glm::vec2(pos.x-1,pos.y))) left = Chunks[glm::vec2(pos.x - 1, pos.y)];
	if (esCarregat(glm::vec2(pos.x + 1, pos.y))) right = Chunks[glm::vec2(pos.x + 1, pos.y)];
	if (esCarregat(glm::vec2(pos.x, pos.y - 1))) down = Chunks[glm::vec2(pos.x, pos.y - 1)];
	if (esCarregat(glm::vec2(pos.x, pos.y + 1))) up = Chunks[glm::vec2(pos.x, pos.y + 1)];
	nou->afegirVeins(left, right, up, down);
	vector<pair<int, glm::vec3>> estructures = nou->emplenarChunk(tipusMon);
	/*for (int i = 0; i < estructures.size(); i++) {
		int tipus = estructures[i].first;
		glm::vec3 pos = estructures[i].second;

		if (tipus == 0) arbrets.push_back(pos);
		else if (tipus == 1) {
			int tipusFlor = rand() % flors.size();
			canviarCub(pos.x, pos.y, pos.z, flors[tipusFlor], false);
		}
	}*/
	calculaLlumNatural(pos);
	nou->preparat = true;
	ChunksReady.push_back({pos,nou});
}

void SuperChunk::calculaLlumNatural(const glm::vec2& pos)
{
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Z; j++) {
			calculaLlumNatural(i + pos.x * X, j + pos.y * Z);
		}
	}
}

void SuperChunk::calculaLlumNatural(int x, int z)
{
	if (DEBUG) return;

	int j = Y - 1;
	uint8_t llum = 15;
	while (blocs.getBloc(obtenirCub(x, j, z))->transparent && j>0) {
		uint8_t tipus = obtenirCub(x, j, z);
		if (tipus != AIRE && !blocs.getBloc(obtenirCub(x, j, z))->vegetacio) llum--;
		canviarLlumNaturalCub(x, j, z, llum);
		j--;
	}

	if (!carregat) return;

	queue<glm::vec3> cua;
	cua.emplace(glm::vec3(x,j+1,z));
	Bloc* b;
	
	while (j >= 0) {
		uint8_t tipus = obtenirCub(x, j, z);
		b = blocs.getBloc(obtenirCub(x, j, z));
		if (b->transparent) cua.emplace(glm::vec3(x, j, z));
		j--;
	}

	glm::vec3 pos;
	set<string> visitades;
	while (!cua.empty()) {
		pos = cua.front();
		cua.pop();
		visitades.insert(glm::to_string(pos));
		uint8_t max = 1, llumActual = obtenirLlumNaturalCub(pos.x,pos.y,pos.z);
		vector<pair<glm::vec3,uint8_t>> colindantes;
		for (int i = 0; i < 6; i++) {
			glm::vec3 actual = glm::vec3(pos.x + posicions[i].x, pos.y + posicions[i].y, pos.z + posicions[i].z);

			if (!blocs.getBloc(obtenirCub(actual.x, actual.y, actual.z))->transparent) continue;

			llum = obtenirLlumNaturalCub(actual.x,actual.y,actual.z);

			if ((llum < llumActual || llum == 0) && visitades.find(glm::to_string(actual)) == visitades.end()) cua.emplace(actual);
			else if (llum > llumActual) colindantes.push_back({ actual,llum });
			if (llum > max) {
				max = llum;
			}
				
			
		}
	
		for (pair<glm::vec3,uint8_t> actual : colindantes) {
			if(actual.second < max) cua.emplace(actual.first);
		}
		canviarLlumNaturalCub(pos.x, pos.y, pos.z, max-1);
	}
}

// LLUM ARTIFICIAL

void SuperChunk::posarLlum(glm::vec3 pos, uint8_t llum) {
	// Mirem si �s transparent i si hem de pujar la llum
	if (blocs.getBloc(obtenirCub(pos.x, pos.y, pos.z))->transparent && obtenirLlumArtificialCub(pos.x, pos.y, pos.z) + 2 <= llum) {
		uint8_t resLlum = llum - 1;
		canviarLlumArtificialCub(pos.x, pos.y, pos.z, resLlum);
		cuaLlum.emplace(pos);
	}
}

void SuperChunk::eliminarLlum(glm::vec3 pos, uint8_t llum) {
	uint8_t llumCub = obtenirLlumArtificialCub(pos.x, pos.y, pos.z);
	if (llumCub != 0 && llumCub < llum) {
		canviarLlumArtificialCub(pos.x, pos.y, pos.z, 0);
		cuaLlumTreure.emplace(pair<glm::vec3, uint8_t>(pos,llumCub));
	}
	else if (llumCub >= llum) {
		llums.emplace(pos);
	}
}

void SuperChunk::afegirLlum(const glm::vec3 posLlum)
{
	// Fem una cua de posicions i afegim la llum
	cuaLlum.emplace(posLlum);

	while (!cuaLlum.empty()) {
		glm::vec3 bloc = cuaLlum.front();
		cuaLlum.pop();
		uint8_t llum = obtenirLlumArtificialCub(bloc.x, bloc.y, bloc.z);
		posarLlum(bloc + glm::vec3(1, 0, 0), llum);
		posarLlum(bloc + glm::vec3(-1, 0, 0), llum);
		posarLlum(bloc + glm::vec3(0, 1, 0), llum);
		posarLlum(bloc + glm::vec3(0, -1, 0), llum);
		posarLlum(bloc + glm::vec3(0, 0, 1), llum);
		posarLlum(bloc + glm::vec3(0, 0, -1), llum);

	}
}

void SuperChunk::treureLlum(const glm::vec3 posLlum, uint8_t llumIni)
{
	cuaLlumTreure.emplace(pair<glm::vec3, uint8_t>(posLlum,llumIni));
	canviarLlumArtificialCub(posLlum.x, posLlum.y, posLlum.z, 0);

	while (!cuaLlumTreure.empty()) {
		glm::vec3 bloc = cuaLlumTreure.front().first;
		uint8_t llum = cuaLlumTreure.front().second;
		cuaLlumTreure.pop();

		eliminarLlum(bloc + glm::vec3(1, 0, 0), llum);
		eliminarLlum(bloc + glm::vec3(-1, 0, 0), llum);
		eliminarLlum(bloc + glm::vec3(0, 1, 0), llum);
		eliminarLlum(bloc + glm::vec3(0, -1, 0), llum);
		eliminarLlum(bloc + glm::vec3(0, 0, 1), llum);
		eliminarLlum(bloc + glm::vec3(0, 0, -1), llum);

	}

	while (!llums.empty()) {
		afegirLlum(llums.front());
		llums.pop();
	}
}

// LLUM NATURAL

void SuperChunk::posarLlumNatural(glm::vec3 pos, uint8_t llum, bool avall) {
	// Mirem si �s transparent (de moment 0, aire) i si hem de pujar la llum
	if (obtenirCub(pos.x, pos.y, pos.z) == AIRE && obtenirLlumNaturalCub(pos.x, pos.y, pos.z) + 2 <= llum) {
		uint8_t resLlum = llum - 1;
		if(avall && llum == llumNatural) resLlum = llum;
		canviarLlumNaturalCub(pos.x, pos.y, pos.z, resLlum);
		cuaLlumNatural.emplace(pos);
	}
}

void SuperChunk::afegirLlumNatural(const glm::vec3 posLlum)
{
	// Fem una cua de posicions i afegim la llum
	cuaLlumNatural.emplace(posLlum);

	while (!cuaLlumNatural.empty()) {
		glm::vec3 bloc = cuaLlumNatural.front();
		cuaLlumNatural.pop();
		uint8_t llum = obtenirLlumNaturalCub(bloc.x, bloc.y, bloc.z);
		posarLlumNatural(bloc + glm::vec3(1, 0, 0), llum);
		posarLlumNatural(bloc + glm::vec3(-1, 0, 0), llum);
		posarLlumNatural(bloc + glm::vec3(0, 1, 0), llum, true);
		posarLlumNatural(bloc + glm::vec3(0, -1, 0), llum, true);
		posarLlumNatural(bloc + glm::vec3(0, 0, 1), llum);
		posarLlumNatural(bloc + glm::vec3(0, 0, -1), llum);

	}
}


void SuperChunk::canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar, bool jugador, char* color)
{
	if (esValid(x,y,z) && (reemplacar || obtenirCub(x, y, z) == AIRE)) {
		//cout << x % X << " " << z % Z << endl;
		Chunk* chunk = Chunks[BlocChunk(x,z)];
		uint8_t tipusBlocAbans = chunk->obtenirCub(x % X, y, z % Z);
		chunk->canviarCub(x % X, y, z % Z, tipus, reemplacar, color);

		//cout << x << " " << y << " " << z << endl;

		if (tipus == LLUM) {
			// Afegim la llum
			canviarLlumArtificialCub(x, y, z, 14);
			afegirLlum(glm::vec3(x, y, z));
			llistaLlums.push_back(glm::vec3(x, y, z));
		}
		else if (tipus == AIRE) {

			if (tipusBlocAbans == LLUM) {
				treureLlum(glm::vec3(x, y, z), 14);
				llistaLlums.remove(glm::vec3(x, y, z));
			}
			else treureLlum(glm::vec3(x, y, z), 0);
		}
		else {
			// NO ES EFICIENT!!!
			for (const glm::vec3 llum : llistaLlums) {
				treureLlum(llum, 14);
				canviarLlumArtificialCub(llum.x, llum.y, llum.z, 14);
				afegirLlum(llum);
			}
			if(!blocs.getBloc(tipus)->transparent) canviarLlumNaturalCub(x, y, z, 0);
		}

		if(jugador) calculaLlumNatural(x, z);

		//chunk.unCanviat = true;
		//chunk.cubCanviat = glm::vec3(x % X, y, z % Z);
	}
	//cout << *color << " " << (int)tipus << endl;
	//cout << "Chunk: " << x/X << ", " << z/Z << "    " << x % X << ", " << z % Z << endl;
}

void SuperChunk::canviarLlumNaturalCub(int x, int y, int z, uint8_t llum)
{
	if (esValid(x,y,z)) {

		Chunk* chunk = Chunks[BlocChunk(x,z)];
		chunk->canviarLlumNaturalCub(x % X, y, z % Z, llum);

	}
}

void SuperChunk::canviarLlumArtificialCub(int x, int y, int z, uint8_t llum)
{
	if (esValid(x,y,z)) {

		Chunk* chunk = Chunks[BlocChunk(x,z)];
		chunk->canviarLlumArtificialCub(x % X, y, z % Z, llum);

	}
}

uint8_t SuperChunk::obtenirCub(int x, int y, int z) const
{
	if (esValid(x, y, z))
		return Chunks[BlocChunk(x,z)]->obtenirCub(x % X, y, z % Z);
	return 0;
}

uint8_t SuperChunk::obtenirLlumNaturalCub(int x, int y, int z)
{
	if (esValid(x,y,z))
		return Chunks[BlocChunk(x,z)]->obtenirLlumNaturalCub(x % X, y, z % Z);
	return 0;
}

uint8_t SuperChunk::obtenirLlumArtificialCub(int x, int y, int z)
{
	if (esValid(x,y,z))
		return Chunks[BlocChunk(x,z)]->obtenirLlumArtificialCub(x % X, y, z % Z);
	return 0;
}

void SuperChunk::BoundingBox(int x, int y, int z)
{
	if (!obtenirCub(x, y, z)) return;
	renderer->activaBounding(1);
	x -= X*DISTANCIA*2-X;
	z -= Z*DISTANCIA*2-Z;
	glBindVertexArray(VAO);
	GLfloat vertices[] = {
	  0.0 + x, 0.0 + y, 0.0 + z, 1.0,
	  1.0 + x, 0.0 + y, 0.0 + z, 1.0,
	  1.0 + x, 1.0 + y, 0.0 + z, 1.0,
	  0.0 + x, 1.0 + y, 0.0 + z, 1.0,
	  0.0 + x, 0.0 + y, 1.0 + z, 1.0,
	  1.0 + x, 0.0 + y, 1.0 + z, 1.0,
	  1.0 + x, 1.0 + y, 1.0 + z, 1.0,
	  0.0 + x, 1.0 + y, 1.0 + z, 1.0,
	};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
	  0, 1, 2, 3,
	  4, 5, 6, 7,
	  0, 4, 1, 5,
	  2, 6, 3, 7
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute
		4,                  // number of elements per vertex, here (x,y,z,w)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);
	renderer->activaBounding(0);
	glBindVertexArray(0);

}

bool SuperChunk::existeixCub(int x, int y, int z, uint8_t tipus) const
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			for (int k = -1; k <= 1; k++)
			{
				if (i == 0 && j == 0 && k == 0) continue;
				if (obtenirCub(x + i, y + j, z + k) == tipus) return true;
			}
		}
	}
	return false;
}

void SuperChunk::update()
{
	
}

void SuperChunk::render()
{	
	if (renderer) {

		renderer->usarShader(0);
		renderer->usarTexturaMon();
		glBindVertexArray(VAO);

		//loadedChunksMutex.lock();
		auto start = std::chrono::high_resolution_clock::now();
		for (auto chunk : Chunks) {
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = end - start;
			if (chunk.second == NULL or chunk.second->descarregant) continue;
			std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);
			// Hem de moure el chunk per tal que no estiguin tots al mateix lloc
			//glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(i * X - X * XC / 2, -Y/2, j * Z - Z * YC / 2));
			glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(chunk.first.x * X, 0, chunk.first.y * Z));
			renderer->colocarMat4("model", model);
			chunk.second->render();
		}
		tempsCarrega = 0;
		//loadedChunksMutex.unlock();
		glBindVertexArray(0);
	}
	
}

bool SuperChunk::renderCub(int x, int y, int z)
{
	glBindVertexArray(VAO);
	if (esValid(x,y,z)) {
		// Hem d'aplicar la mateixa transformaci� que abans
		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x/X * X, 0, z/Z * Z));
		renderer->colocarMat4("model", model);

		return Chunks[BlocChunk(x,z)]->renderCub(x % X, y, z % Z);

	}
	return false;
}

void SuperChunk::arbre(int x, int y, int z)
{
	// Tronco
	const int MIN_TRONCO = 5, MAX_TRONCO = 3;
	int max = MIN_TRONCO + (rand() % MAX_TRONCO);
	for (int i = 0; i < max; i++)
	{
		canviarCub(x, y + i, z, FUSTA);
	}
	char* color = (char*)"VerdFulles";
	// Copa del arbol
	emplenar(x, y + max, z, 1, 1, FULLES, 0, false,color);
	int segY = y + max - 1;
	float probabilitat = 0.3;
	emplenar(x, segY, z, 1, 1, FULLES, probabilitat, false,color);
	segY--;
	emplenar(x, segY, z, 2, 2, FULLES, probabilitat, false,color);
	segY--;                                                                                                                                                                                                                                              
	emplenar(x, segY, z, 2, 2, FULLES, probabilitat, false,color);
}

void SuperChunk::emplenar(int x, int y, int z, int amplitut, int llargada, uint8_t tipus, float probabilitat, bool reemplacar, char* color) {
	for (int i = -amplitut; i <= amplitut; i++) {
		for (int j = -llargada; j <= llargada; j++) {
			if (abs(i) != amplitut || abs(i) != abs(j) || (float)(rand()) / (float)(RAND_MAX) <= probabilitat) {
				canviarCub(x + i, y, z + j, tipus, reemplacar,false,color);
			}
			if (abs(i) != amplitut || abs(i) != abs(j) || (float)(rand()) / (float)(RAND_MAX) <= probabilitat) {
				canviarCub(x - i, y, z - j, tipus, reemplacar,false,color);
			}

		}
	}
}

void SuperChunk::emplenarArea(int x1, int y1, int z1, int x2, int y2, int z2, uint8_t tipus, bool reemplacar, char* color)
{
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			for (int k = z1; k <= z2; k++) {
				canviarCub(i,j,k,tipus,reemplacar,false,color);
			}
		}
	}
}

vector<glm::vec3> SuperChunk::obtenirColindants(const glm::vec3& pos, bool transparents) const
{
	vector<glm::vec3> res;
	for (int i = 0; i < 6; i++) {
		glm::vec3 act = glm::vec3(pos.x+posicions[i].x, pos.y + posicions[i].y, pos.z + posicions[i].z);
		bool valid = esValid(act);
		if(valid && !transparents || valid && blocs.getBloc(obtenirCub(act.x,act.y,act.z))->transparent) res.push_back(act);
	}
	return res;
}

bool SuperChunk::esValid(int x, int y, int z) const
{
	//return x>=0 && x / X < XC && y>=0 && y<Y && z>=0 && z / Z < YC;
	return esCarregat(BlocChunk(x, z));

}

bool SuperChunk::esValid(const glm::vec3& pos) const
{
	return esValid(pos.x,pos.y,pos.z);
}

glm::vec2 SuperChunk::BlocChunk(const glm::vec3& pos) const
{
	return BlocChunk(pos.x,pos.z);
}

glm::vec2 SuperChunk::BlocChunk(int x, int z) const
{
	return glm::vec2(floor(x / X), floor(z / Z));
}

bool SuperChunk::esCarregat(const glm::vec2& pos) const
{
	return Chunks.count(pos) > 0 && Chunks[pos] != NULL;
}

bool SuperChunk::existeixCua(const deque<glm::vec2>& cua, const glm::vec2& e) const
{
	std::lock_guard<std::recursive_mutex> lock(cuaMutex);
	deque<glm::vec2>::const_iterator it = find(cua.begin(), cua.end(), e);
	return it != cua.end();
}
