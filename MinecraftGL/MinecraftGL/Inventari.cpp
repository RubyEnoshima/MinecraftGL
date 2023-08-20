#include "Inventari.h"

Inventari::Inventari()
{
	mapaItems = Recursos::obtTextura("items.png");
	mapaBlocs = Recursos::obtTextura("minecraft_transp.png");

	for (int i = 0; i < MAX_ITEMS; i++) {
		inventari.push_back(new Slot(mapaItems,i));
	}
	for (int i = 0; i < MAX_FILES; i++) {
		inventariGran.push_back(vector<Slot*>());
		for (int j = 0; j < MAX_ITEMS; j++) {
			inventariGran[i].push_back(new Slot(mapaItems, j + 10 * i + 10));
		}
	}
	slotMouse = new Slot(mapaItems, -1);
	initRenderData();
}

Inventari::~Inventari()
{
	for (auto slot : inventari)
	{
		delete slot;
	}
	for (auto vector : inventariGran)
	{
		for (auto slot : vector)
		{
			delete slot;
		}
	}
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete shader;
	delete slotMouse;
}

void Inventari::iniciaSprites(SpriteRenderer* _renderer, TextRenderer* _text)
{
	text = _text;
	renderer = _renderer;
	Sprite* spriteInventari = new Sprite(Recursos::obtTextura("inventari.png"), "Inventari", glm::vec2(), glm::vec2(7), true);
	spriteInventari->teletransportar(glm::vec2(renderer->width / 2, renderer->height - spriteInventari->obtTamany().y * 1.7));
	renderer->afegirSprite(spriteInventari);

	spriteSlot = new Sprite(Recursos::obtTextura("seleccio.png"), "Seleccio", glm::vec2(), glm::vec2(7), true);
	spriteSlot->teletransportar(glm::vec2(renderer->width / 2 - 80 * 3.5, renderer->height - spriteSlot->obtTamany().y * 1.7));
	posInicial = spriteSlot->obtPos();
	spriteSlot->indexZ = 3;
	renderer->afegirSprite(spriteSlot);

	Sprite* fons = new Sprite(Recursos::obtTextura("seleccio.png"), "Fons", glm::vec2(), glm::vec2(renderer->width, renderer->height));
	fons->color = { 0.1,0.1,0.1,0.5 };
	fons->posicioMapa = { 2, 0 };
	fons->tamanyMapa = { 1,1 };
	fons->indexZ = -1;
	fons->visible = false;
	renderer->afegirSprite(fons);

	Sprite* spriteCreatiu = new Sprite(Recursos::obtTextura("creatiu.png"), "Creatiu", glm::vec2(renderer->width / 2, renderer->height / 2), glm::vec2(7), true);
	spriteCreatiu->indexZ = 1;
	spriteCreatiu->visible = false;
	renderer->afegirSprite(spriteCreatiu);

	Sprite* _caixeta = new Sprite(Recursos::obtTextura("caixeta.png"), "Caixeta", glm::vec2(renderer->width / 2, renderer->height / 2), glm::vec2(4.2,3), true);
	_caixeta->indexZ = 50;
	_caixeta->centrar(false);
	//renderer->afegirSprite(_caixeta);
	caixeta = _caixeta;


	for (auto slot : inventari)
	{
		slot->sprite->teletransportar(glm::vec2(renderer->width / 2 - 70 * 4 + 70 * slot->id, renderer->height - 30 - 5));
		renderer->afegirSprite(slot->sprite);
	}
	int i = 1;
	for (auto vector : inventariGran)
	{
		int j = 1;
		for (auto slot : vector)
		{
			float q = (slot->id - 10 * i);
			slot->sprite->teletransportar(glm::vec2(renderer->width / 2 - 70.75 * 4 + 63 * (slot->id - 10 * i), renderer->height/2 - 19 - 191 + 64 * i*0.99));
			renderer->afegirSprite(slot->sprite);
			j++;
		}
		i++;
	}
	renderer->afegirSprite(slotMouse->sprite);
	if(creatiu) emplenarInventariGran();
}

void Inventari::canviaSeleccionat(const uint8_t seleccio)
{
	if (dintre) return;
	if (seleccio >= 0 && seleccio < MAX_ITEMS) {
		slotSeleccionat = seleccio;
		glm::vec2 novaPos = posInicial + glm::vec2(seleccio * 70, 0);
		spriteSlot->teletransportar(novaPos);
	}
}

void Inventari::canviaSelecccionatPer1(int seleccio)
{
	if (dintre) return;

	slotSeleccionat += seleccio;
	if (slotSeleccionat < 0) slotSeleccionat = MAX_ITEMS - 1;
	else if (slotSeleccionat >= MAX_ITEMS) slotSeleccionat = 0;
	canviaSeleccionat(slotSeleccionat);
}

Item* Inventari::obtenirItemActual() const
{
	return Recursos::getItem(inventari[slotSeleccionat]->obtItem());
}

void Inventari::afegirItem(int id, uint8_t _quantitat)
{
	if (id == -1) return;
	if (quantitat == MAX_ITEMS) return;
	int i = 0;
	while (i < MAX_ITEMS) {
		Slot* actual = inventari[i];
		if (actual->obtItem() == id) {
			actual->quantitat += _quantitat;
			break;
		}
		else if (actual->obtItem() == -1) {
			actual->setItem(id);
			actual->quantitat = _quantitat;
			quantitat++;

			break;
		}
		i++;
	}
}

void Inventari::afegirItem(string nom, uint8_t _quantitat)
{
	Item* i = Recursos::getItem(nom);
	if (i == NULL) return;
	afegirItem(i->id,_quantitat);
}

void Inventari::alternaVisibilitat()
{
	visible = !visible;
}

void Inventari::obrir()
{
	dintre = !dintre;
	renderer->obtSprite("Seleccio")->visible = !dintre;
	renderer->obtSprite("Inventari")->visible = !dintre;
	renderer->obtSprite("Fons")->visible = dintre;
	renderer->obtSprite("Creatiu")->visible = dintre;
	for (auto slot : inventari)
	{
		if (slot->obtItem() == -1) continue;
		if(dintre) slot->sprite->teletransportar(glm::vec2(renderer->width / 2 - 70.75 * 4 + 63 * (slot->id), renderer->height / 2 - 18 - 191 + 64 * 6.125));
		else slot->sprite->teletransportar(glm::vec2(renderer->width / 2 - 70 * 4 + 70 * slot->id, renderer->height - 32 - 5));
	}
	for (auto fila : inventariGran) {
		for (auto slot : fila) {
			int item = slot->obtItem();
			if (item == -1) continue;
			slot->visible = dintre;
			if (Recursos::getItem(item)->classe == BLOC && Recursos::getBloc(item)->vegetacio) slot->sprite->visible = dintre;
		}
	}
	if (dintre) {
	}
	else if (!dintre) {
		if (slotMouse->obtItem() != -1) {
			ultim->setItem(slotMouse->obtItem());
			ultim->quantitat = slotMouse->quantitat;
			slotMouse->setItem(-1);
			slotMouse->sprite->visible = false;
			ultim = NULL;
		}
		sobre = NULL;
	}
}

void Inventari::agafarItem()
{
	if (sobre) {
		int i = sobre->obtItem();
		if (slotMouse->obtItem() != -1) { // Deixar-lo 
			if (i == slotMouse->obtItem()) {
				slotMouse->setItem(-1);
				sobre->quantitat += slotMouse->quantitat;
				slotMouse->quantitat = 0;
			}
			else {
				// Intercanviar amb l'slot
				uint8_t aux = sobre->quantitat;
				sobre->setItem(slotMouse->obtItem());
				sobre->quantitat = slotMouse->quantitat;
				slotMouse->setItem(i);
				slotMouse->quantitat = aux;

			}
			if (i == -1) ultim = NULL;
		}
		else if (i != -1) { // Agafar l'item
			slotMouse->setItem(i);
			slotMouse->quantitat = sobre->quantitat;
			sobre->setItem(-1);
			sobre->quantitat = 0;
			ultim = sobre;
		}

	}
}

void Inventari::render(const glm::vec2& mousePos)
{
	if (!visible) return;

	if (!dintre) {
		for (auto slot : inventari)
		{
			if (!slot->visible || slot->obtItem() == -1) continue;

			// Renderitzem la quantitat d'objectes a l'slot
			if(slot->quantitat > 1) text->RenderText(to_string(slot->quantitat), slot->sprite->obtPos() + glm::vec2(10, 6), 0.16,false,glm::vec3(1),true);
			
			mapaBlocs->use();
			slot->render(shader,VAO);
		}
	}
	else {
		bool mouse = false;

		for (auto slot : inventari)
		{
			//if (!slot->visible || slot->obtItem() == -1) continue;
			
			if (!mouse && dintre && slot->mouseSobre(mousePos)) {
				if (slot->obtItem() != -1) {
					caixeta->visible = true;
					caixeta->teletransportar(mousePos - glm::vec2(0, caixeta->tamanyMapa.y + 15));

				}
				mouse = true;
				sobre = slot;
			}
		}
		for (auto fila : inventariGran) {
			for (auto slot : fila) {
				//if (slot == agafat) continue;
				//if (slot->obtItem() == -1) continue;

				if (!mouse && slot->mouseSobre(mousePos)) {
					if (slot->obtItem() != -1) {
						caixeta->visible = true;
						caixeta->teletransportar(mousePos - glm::vec2(0, caixeta->tamanyMapa.y + 15));

					}
					mouse = true;
					sobre = slot;
				}
			}
		}
		if (!mouse) sobre = NULL;
		if (sobre == NULL || sobre && sobre->obtItem() == -1) {
			caixeta->visible = false;
		}
		if(slotMouse->obtItem() == -1) renderer->DrawSprite(caixeta);
		else{
			slotMouse->sprite->teletransportar(mousePos);
			slotMouse->render(shader, VAO);
		}

		for (auto slot : inventari)
		{
			if (!slot->visible || slot->obtItem() == -1) continue;

			// Renderitzem la quantitat d'objectes a l'slot
			if (slot->quantitat > 1) text->RenderText(to_string(slot->quantitat), slot->sprite->obtPos() + glm::vec2(10, 6), 0.16, false, glm::vec3(1), true);

			mapaBlocs->use();
			slot->render(shader, VAO);
		}
		for (auto fila : inventariGran) {
			for (auto slot : fila) {
				if (slot->obtItem() == -1) continue;
				mapaBlocs->use();

				slot->render(shader,VAO);
			}
		}
		

		text->RenderText("Inventari creatiu", { renderer->width / 2 - 310,renderer->height / 2 - 215 }, 0.175, false, glm::vec3(1), true);
		if (sobre && sobre->obtItem() != -1 && slotMouse->obtItem() == -1) text->RenderText(Recursos::getItem(sobre->obtItem())->nom, caixeta->obtPos() + glm::vec2(10, caixeta->tamanyMapa.y / 2 - 1), 0.15, false, glm::vec3(1), true, 1);

	}
	
	
}

void Inventari::initRenderData()
{
	shader = new ShaderProgram("VertexBlocItem.vert","FragmentBlocItem.frag");
	shader->carregaShaders();
	shader->colocarInt("image", 1);
	float vertices[] = {
		// pos      // tex
		0.0f, 0.25f, 0.0f, 1.0f,	   0.37,
		0.5f, 0.0f, 1.0f, 1.0f,		   0.37,
		0.0f, 0.75f, 0.0f, 0.0f,	   0.37,
									   
		0.0f, 0.75f, 0.0f, 0.0f,	   0.37,
		0.5f, 0.0f, 1.0f, 1.0f,		   0.37,
		0.5f, 0.525f, 1.0f, 0.0f,	   0.37,
									   	
		// pos      // tex			   	
		0.5f, 0.0f, 0.0f, 1.0f,		   0.5,
		1.0f, 0.25f, 1.0f, 1.0f,	   0.5,
		0.5f, 0.525f, 0.0f, 0.0f,	   0.5,
									   	
		0.5f, 0.525f, 0.0f, 0.0f,	   0.5,
		1.0f, 0.25f, 1.0f, 1.0f,	   0.5,
		1.0f, 0.75f, 1.0f, 0.0f,	   0.5,
									   	
		// pos      // tex			   	
		0.0f, 0.75f, 0.0f, 1.0f,	   1,
		0.5f, 0.525f, 1.0f, 1.0f,	   1,
		0.5f, 1.0f, 0.0f, 0.0f,		   1,
									   	
		0.5f, 1.0f, 0.0f, 0.0f,		   1,
		0.5f, 0.525f, 1.0f, 1.0f,	   1,
		1.0f, 0.75f, 1.0f, 0.0f,	1
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Inventari::emplenarInventariGran()
{
	int quants = 0;
	for (int i = 0; i < MAX_BLOCS; i++) {
		if (quants >= MAX_ITEMS * MAX_FILES) return;
		Item* item = Recursos::getItem(i);
		if (item) {
			inventariGran[quants / MAX_ITEMS][quants % MAX_ITEMS]->setItem(item->id);
			inventariGran[quants / MAX_ITEMS][quants % MAX_ITEMS]->quantitat = 1;
			quants++;
		}
		
	}
}

void Slot::actualitzaSprite()
{
	if (item == -1) {
		sprite->visible = false;
		visible = false;
		return;
	}
	visible = true;
	Item* i = Recursos::getItem(item);
	if (i->classe == BLOC) {
		Bloc* b = Recursos::getBloc(item);
		if (b->vegetacio) {
			sprite->color = glm::vec4(1.0f);
			sprite->textura = Recursos::obtTextura("minecraft_transp.png");
			sprite->posicioMapa = glm::vec2(b->id * 16, (b->id/16 * 16));
		}
		else {
			if (i->id == GESPA) {
				glm::vec3* color = Recursos::obtColor(Recursos::VERDGESPA);
				sprite->color = glm::vec4(color->r/255,color->g/255,color->b/255, 1.0f);
			}
			else if (i->id == FULLES) {
				glm::vec3* color = Recursos::obtColor(Recursos::VERDFULLES);
				sprite->color = glm::vec4(color->r / 255, color->g / 255, color->b / 255, 1.0f);
			}
			else sprite->color = glm::vec4(1.0f);

		}
		sprite->visible = false;
		return;
	}
	sprite->textura = Recursos::obtTextura("items.png");
	sprite->color = glm::vec4(1.0f);
	uint8_t posSprite = i->sprite;
	sprite->posicioMapa = glm::vec2((posSprite % 16) * 16, (int)(posSprite / 16) * 16);
	sprite->visible = true;
}

bool Slot::mouseSobre(const glm::vec2& posMouse) const
{
	glm::vec2 pos = sprite->obtPos();
	return posMouse.x >= pos.x - 32 && posMouse.y >= pos.y - 32 && posMouse.x <= pos.x + 32 && posMouse.y <= pos.y + 30 ;
}

void Slot::render(ShaderProgram* shader, int VAO)
{
	Item* i = Recursos::getItem(item);
	if (i->classe != BLOC) return; // Només renderitzem blocs, els sprites ja s'encarrega l'SpriteRenderer
	Bloc* b = Recursos::getBloc(i->id);
	if (b->vegetacio) {
		sprite->visible = true;
		return;
	}
	shader->usar();
	glm::mat4 model = sprite->model;
	//model = glm::scale(model, glm::vec3(.75));
	shader->colocarMat4("model", model);


	shader->colocarVec2("posicioSprite", glm::vec2(16 * (b->costats % 16), 16 * (int)(b->costats / 16)));
	shader->colocarVec2("tamanyTextura", sprite->textura->obtTamany());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	shader->colocarVec4("spriteColor", sprite->color);
	shader->colocarVec2("posicioSprite", glm::vec2(16 * (b->adalt % 16), 16 * (int)(b->adalt / 16)));
	glDrawArrays(GL_TRIANGLES, 12, 6);
	shader->colocarVec4("spriteColor", glm::vec4(1));
	glBindVertexArray(0);
}
