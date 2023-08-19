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
	std::lock_guard<std::recursive_mutex> lock(descarregarMutex);

	for (auto chunk : Chunks)
	{
		if(!chunk.second->descarregant) delete chunk.second;
	}
	glDeleteBuffers(1, &VAO);
	for(auto noise : noises) delete noise.noise;
}

SuperChunk::SuperChunk(Renderer* _renderer)
{
	srand(semilla);
	Soroll continentalness;
	continentalness.noise = new FastNoiseLite(semilla);
	continentalness.noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	continentalness.noise->SetFrequency(0.0075);
	continentalness.noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	continentalness.noise->SetFractalLacunarity(2);
	continentalness.noise->SetFractalGain(0.3);
	continentalness.noise->SetFractalOctaves(7);
	continentalness.importancia = 0.75;
	//continentalness.punts = { glm::vec2(-1,5), glm::vec2(-0.25,5), glm::vec2(-0.15,50), glm::vec2(0.1,50), glm::vec2(0.12,90), glm::vec2(0.13,90), glm::vec2(0.15,100), glm::vec2(0.5,120), glm::vec2(1,125) };
	continentalness.punts = { glm::vec2(-1,0), glm::vec2(0,70), glm::vec2(0.5,95), glm::vec2(0.7,105), glm::vec2(1,110) };

	noises.push_back(continentalness);

	Soroll erosion;
	erosion.noise = new FastNoiseLite(semilla);
	erosion.noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	erosion.noise->SetFrequency(0.018);
	erosion.noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	erosion.noise->SetFractalLacunarity(0.05);
	erosion.noise->SetFractalGain(0.8);
	erosion.noise->SetFractalOctaves(3);
	erosion.importancia = 0.5;
	erosion.punts = { glm::vec2(-1,0), glm::vec2(0,15), glm::vec2(0.75,20), glm::vec2(1,20) };
	noises.push_back(erosion);

	//Soroll pv;
	//pv.noise = new FastNoiseLite(semilla);
	//pv.noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	//pv.noise->SetFrequency(0.05);
	//pv.noise->SetFractalType(FastNoiseLite::FractalType_Ridged);
	//pv.noise->SetFractalLacunarity(1);
	//pv.noise->SetFractalGain(0.2);
	//pv.noise->SetFractalOctaves(2);
	//pv.importancia = 0.1;
	////pv.punts = { glm::vec2(-1,5), glm::vec2(-0.75,20), glm::vec2(-0.25,30), glm::vec2(0,35), glm::vec2(0.5,100), glm::vec2(1,100) };
	//pv.punts = { glm::vec2(-1,0), glm::vec2(-0.75,50), glm::vec2(0.5,40), glm::vec2(1,20) };
	//noises.push_back(pv);
	
	renderer = _renderer;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	vector<glm::vec3> arbrets;

	//carregat = true;
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
		std::lock_guard<std::recursive_mutex> lock(descarregarMutex);

		Chunks.erase(chunk);

	}

	
	// MIRAR AL VOLTANT DEL JUGADOR Y POSAR EN CUA CHUNKS NOUS
	if (!esCarregat(chunkJugador) && !existeixCua(chunksCarregar, chunkJugador)) {
		chunksCarregar.push_front(chunkJugador);
		chunkInicial = chunkJugador;
	}
	for (int i = -DISTANCIA; i <= DISTANCIA; i++) {
		for (int j = -DISTANCIA; j <= DISTANCIA; j++) {
			if (abs(i) + abs(j) > DISTANCIA) continue;
			glm::vec2 nouChunk = chunkJugador + glm::vec2(i,j);
			if (!esCarregat(nouChunk) && !existeixCua(chunksCarregar, nouChunk))chunksCarregar.push_front(nouChunk);
				
		}
	}
	//cout << "--------" << endl;

}

void SuperChunk::update(const glm::vec2& chunkJugador, const glm::mat4& mvp)
{
	std::lock_guard<std::recursive_mutex> lock(descarregarMutex);

	for (int i = -DISTANCIA+1; i < DISTANCIA; i++) {
		for (int j = -DISTANCIA+1; j < DISTANCIA; j++) {
			if (abs(i) + abs(j) > DISTANCIA) continue;
			glm::vec2 chunk = chunkJugador + glm::vec2(i, j);

			if (esCarregat(chunk)) {
				Chunks[chunk]->crearVertexs();
				if (chunk == chunkInicial) carregat = true;
			}
		}
	}
}

void SuperChunk::descarregarChunks()
{
	if (!chunksDescarregar.empty()) {
		for (int i = 0; i < NCHUNKS; i++) {
			if (chunksDescarregar.empty()) break;
			std::lock_guard<std::recursive_mutex> lock(cuaMutex);

			Chunk* c = chunksDescarregar.back();
			chunksDescarregar.pop_back();

			delete c;
			/*Chunks.erase(pos);*/
		}
	}

}

void SuperChunk::carregarChunks()
{
	if (!chunksCarregar.empty() && potGenerar) {
		//std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);
		for (int i = 0; i < NCHUNKS; i++) {
			if (chunksCarregar.empty()) break;
			std::lock_guard<std::recursive_mutex> lock(cuaMutex);
			glm::vec2 pos = chunksCarregar.back();
			chunksCarregar.pop_back();
			if (!esCarregat(pos)) {
				vector<glm::vec3> arbrets;
				generarChunk(pos, arbrets);
				for (int i = 0; i < arbrets.size(); i++) {
					glm::vec3 posArbre = arbrets[i];
					arbre(posArbre.x, posArbre.y, posArbre.z);
				}

				//potGenerar = false;
			}

		}
	}
}

void SuperChunk::eliminaCarregats()
{
	/*for (int i = 0; i < NCHUNKS; i++) {
		if (ChunksReady.empty()) break;
		std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);

		Chunks[ChunksReady.back().first] = ChunksReady.back().second;
		ChunksReady.pop_back();
	}*/
}

void SuperChunk::generarChunk(const glm::vec2& pos, vector<glm::vec3>&arbrets)
{
	Chunk* nou = new Chunk(pos.x, pos.y);
	Chunk* up = NULL;
	Chunk* left = NULL;
	Chunk* right = NULL; 
	Chunk* down = NULL;
	if (esCarregat(glm::vec2(pos.x-1,pos.y))) left = Chunks[glm::vec2(pos.x - 1, pos.y)];
	if (esCarregat(glm::vec2(pos.x + 1, pos.y))) right = Chunks[glm::vec2(pos.x + 1, pos.y)];
	if (esCarregat(glm::vec2(pos.x, pos.y - 1))) down = Chunks[glm::vec2(pos.x, pos.y - 1)];
	if (esCarregat(glm::vec2(pos.x, pos.y + 1))) up = Chunks[glm::vec2(pos.x, pos.y + 1)];
	nou->afegirVeins(left, right, up, down);
	vector<pair<int, glm::vec3>> estructures = nou->emplenarChunk(tipusMon, noises);
	for (int i = 0; i < estructures.size(); i++) {
		int tipus = estructures[i].first;
		glm::vec3 pos = estructures[i].second;

		if (tipus == 0) arbrets.push_back(pos);
		else if (tipus == 1) {
			int tipusFlor = rand() % flors.size();
			canviarCub(pos.x, pos.y, pos.z, flors[tipusFlor], false);
		}
	}

	calculaLlumNatural(pos);
	nou->preparat = true;
	Chunks[pos] = nou;

	// Mirem blocs que no s'hagin pogut posar, per si de cas algun es troba en aquest chunk
	auto it = blocsNoPosats.find(pos);
	if (it != blocsNoPosats.end()) {
		for (auto& bloc : (*it).second) {
			int color = Recursos::BLANC;
			if (bloc.second == FULLES) color = Recursos::VERDFULLES;
			canviarCub(bloc.first.x, bloc.first.y, bloc.first.z, bloc.second, false, false, color);
		}
		blocsNoPosats.erase(it);
	}
	
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
	while (Recursos::getBloc(obtenirCub(x, j, z))->transparent && j>0) {
		uint8_t tipus = obtenirCub(x, j, z);
		if (tipus != AIRE && !Recursos::getBloc(obtenirCub(x, j, z))->vegetacio) llum--;
		canviarLlumNaturalCub(x, j, z, llum);
		j--;
	}

	if (!carregat) return;

	queue<glm::vec3> cua;
	cua.emplace(glm::vec3(x,j+1,z));
	Bloc* b;
	
	while (j >= 0) {
		uint8_t tipus = obtenirCub(x, j, z);
		b = Recursos::getBloc(obtenirCub(x, j, z));
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

			if (!Recursos::getBloc(obtenirCub(actual.x, actual.y, actual.z))->transparent) continue;

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
	// Mirem si és transparent i si hem de pujar la llum
	if (Recursos::getBloc(obtenirCub(pos.x, pos.y, pos.z))->transparent && obtenirLlumArtificialCub(pos.x, pos.y, pos.z) + 2 <= llum) {
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
	if (DEBUG) return;
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
	if (DEBUG) return;

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
	// Mirem si és transparent (de moment 0, aire) i si hem de pujar la llum
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


void SuperChunk::canviarCub(int x, int y, int z, uint8_t tipus, bool reemplacar, bool jugador, int color)
{
	bool valid = esValid(x, y, z);
	if (valid && (reemplacar || obtenirCub(x, y, z) == AIRE)) {
		Chunk* chunk = Chunks[BlocChunk(x, z)];
		uint8_t tipusBlocAbans = chunk->obtenirCub(Mon2Chunk(x, X), y, Mon2Chunk(z, Z));
		chunk->canviarCub(Mon2Chunk(x, X), y, Mon2Chunk(z, Z), tipus, reemplacar, color);

		//cout << "El bloc " << x << " " << y << " " << z << " es troba al chunk " << BlocChunk(x, z) << " i dins es el bloc " << Mon2Chunk(x, X) << " " << Mon2Chunk(z, Z) << endl;

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
			if (!Recursos::getBloc(tipus)->transparent) canviarLlumNaturalCub(x, y, z, 0);
		}

		if (jugador) calculaLlumNatural(x, z);

		//chunk.unCanviat = true;
		//chunk.cubCanviat = glm::vec3(Mon2Chunk(x,X), y, Mon2Chunk(z,Z));
	}
	else if (!valid) {
		//cout << "No s'ha pogut posar " << (int)tipus << endl;
		auto it = blocsNoPosats.find(BlocChunk(x, z));
		if (it != blocsNoPosats.end()) {
			(*it).second.push_back({ {x,y,z},tipus });
		}
		else blocsNoPosats.insert({ BlocChunk(x, z), {{ {x,y,z},tipus }} });
	}
	//cout << *color << " " << (int)tipus << endl;
	//cout << "Chunk: " << x/X << ", " << z/Z << "    " << Mon2Chunk(x,X) << ", " << Mon2Chunk(z,Z) << endl;
}

void SuperChunk::canviarLlumNaturalCub(int x, int y, int z, uint8_t llum)
{
	if (esValid(x,y,z)) {

		Chunk* chunk = Chunks[BlocChunk(x,z)];
		chunk->canviarLlumNaturalCub(Mon2Chunk(x,X), y, Mon2Chunk(z,Z), llum);

	}
}

void SuperChunk::canviarLlumArtificialCub(int x, int y, int z, uint8_t llum)
{
	if (esValid(x,y,z)) {

		Chunk* chunk = Chunks[BlocChunk(x,z)];
		chunk->canviarLlumArtificialCub(Mon2Chunk(x,X), y, Mon2Chunk(z,Z), llum);

	}
}

uint8_t SuperChunk::obtenirCub(const glm::vec3& pos) const
{
	return obtenirCub(pos.x,pos.y,pos.z);
}

uint8_t SuperChunk::obtenirCub(int x, int y, int z) const
{
	if (esValid(x, y, z))
		return Chunks[BlocChunk(x,z)]->obtenirCub(Mon2Chunk(x,X), y, Mon2Chunk(z,Z));
	return 0;
}

uint8_t SuperChunk::obtenirLlumNaturalCub(int x, int y, int z)
{
	if (esValid(x,y,z))
		return Chunks[BlocChunk(x,z)]->obtenirLlumNaturalCub(Mon2Chunk(x,X), y, Mon2Chunk(z,Z));
	return 0;
}

uint8_t SuperChunk::obtenirLlumArtificialCub(int x, int y, int z)
{
	if (esValid(x,y,z))
		return Chunks[BlocChunk(x,z)]->obtenirLlumArtificialCub(Mon2Chunk(x,X), y, Mon2Chunk(z,Z));
	return 0;
}

void SuperChunk::BoundingBox(int x, int y, int z)
{
	if (!obtenirCub(x, y, z)) return;
	renderer->activaBounding(1);
	glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(BlocChunk(x, z).x * X, 0, BlocChunk(x, z).y * Z));
	renderer->colocarMat4("model", model);
	x -= X*BlocChunk(x,z).x;
	z -= Z* BlocChunk(x, z).y;
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

void SuperChunk::render(Frustum* frustum, bool sotaAigua)
{	
	if (renderer) {

		renderer->usarShader(0);
		renderer->usarTexturaMon();
		glBindVertexArray(VAO);
		
		//loadedChunksMutex.lock();
		for (auto chunk : Chunks) {
			if (chunk.second == NULL || chunk.second->descarregant || (activaFrustum && !chunk.second->esVisible(frustum))) continue;
			std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);
			// Hem de moure el chunk per tal que no estiguin tots al mateix lloc
			glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(chunk.first.x * X, 0, chunk.first.y * Z));
			renderer->colocarMat4("model", model);
			chunk.second->render();
		}

		if(sotaAigua) glDisable(GL_CULL_FACE);
		// RENDERITZAR L'AIGUA
		for (auto chunk : Chunks) {
			if (chunk.second == NULL || chunk.second->descarregant || (activaFrustum && !chunk.second->esVisible(frustum))) continue;
			std::lock_guard<std::recursive_mutex> lock(loadedChunksMutex);
			// Hem de moure el chunk per tal que no estiguin tots al mateix lloc
			glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(chunk.first.x * X, 0, chunk.first.y * Z));
			renderer->colocarMat4("model", model);
			chunk.second->render(true);
		}
		if (sotaAigua) glEnable(GL_CULL_FACE);
		//loadedChunksMutex.unlock();
		glBindVertexArray(0);
	}
	
}

bool SuperChunk::renderCub(int x, int y, int z)
{
	glBindVertexArray(VAO);
	if (esValid(x,y,z)) {
		// Hem d'aplicar la mateixa transformació que abans
		glm::vec2 chunk = BlocChunk(x, z);
		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(chunk.x * X, 0, chunk.y * Z));
		renderer->colocarMat4("model", model);

		return Chunks[chunk]->renderCub(Mon2Chunk(x,X), y, Mon2Chunk(z,Z));

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
	int color = Recursos::VERDFULLES;
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

void SuperChunk::emplenar(int x, int y, int z, int amplitut, int llargada, uint8_t tipus, float probabilitat, bool reemplacar, int color) {
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

void SuperChunk::emplenarArea(int x1, int y1, int z1, int x2, int y2, int z2, uint8_t tipus, bool reemplacar, int color)
{
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			for (int k = z1; k <= z2; k++) {
				canviarCub(i,j,k,tipus,reemplacar,false,color);
			}
		}
	}
}

vector<pair<glm::vec3, uint8_t>> SuperChunk::obtenirColindants(const glm::vec3& pos, int transparents, bool ellMateix) const
{
	vector<pair<glm::vec3, uint8_t>> res;
	int max = 6;
	if (ellMateix) max++;
	for (int i = 0; i < max; i++) {
		glm::vec3 act = glm::vec3(pos.x+posicions[i].x, pos.y + posicions[i].y, pos.z + posicions[i].z);
		//if (!esValid(act)) continue;
		uint8_t tipus = obtenirCub(act.x, act.y, act.z);
		if (transparents == 0) res.push_back({ act, tipus});
		else {
			Bloc* b = Recursos::getBloc(tipus);
			bool esSolid = b->solid;
			if ((transparents == 1 && !esSolid) || (transparents == 2 && esSolid)) res.push_back({ act,tipus });
		}
	}
	return res;
}

vector<AABB> SuperChunk::obtenirAABB(const glm::vec3& pos)
{
	vector<AABB> res;
	vector<pair<glm::vec3, uint8_t>> blocs = obtenirColindants(pos, 2);
	for(const auto& b : blocs)
	{
		AABB aabb;
		//aabb.shader = renderer->obtShader();
		aabb.pos = b.first;
		aabb.tamany = glm::vec3(1, 1, 1);
		res.push_back(aabb);
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
	return glm::vec2(floor((double)x / X), floor((double)z / Z));
}

int SuperChunk::Mon2Chunk(int n, int m) const
{
	// És l'equivalent a fer n % m, però fent que tracti bé els negatius
	return ((n % m) + m) % m;
}

bool SuperChunk::esCarregat(const glm::vec2& pos) const
{
	std::lock_guard<std::recursive_mutex> lock(cuaMutex);

	return Chunks.count(pos) > 0 && Chunks[pos] != NULL && !Chunks[pos]->descarregant && Chunks[pos]->preparat;
}

bool SuperChunk::existeixCua(const deque<glm::vec2>& cua, const glm::vec2& e) const
{
	std::lock_guard<std::recursive_mutex> lock(cuaMutex);
	deque<glm::vec2>::const_iterator it = find(cua.begin(), cua.end(), e);
	return it != cua.end();
}
