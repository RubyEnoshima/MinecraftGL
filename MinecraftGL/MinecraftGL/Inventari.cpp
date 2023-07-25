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
			inventariGran[i].push_back(new Slot(mapaItems, (10 * i + 1 + i * j + j)+9));
		}
	}
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
	//delete mapaItems;
}

void Inventari::iniciaSprites(SpriteRenderer* renderer)
{
	Sprite* spriteInventari = new Sprite(Recursos::obtTextura("inventari.png"), "Inventari", glm::vec2(), glm::vec2(7), true);
	spriteInventari->teletransportar(glm::vec2(renderer->width / 2, renderer->height - spriteInventari->obtTamany().y * 1.7));
	renderer->afegirSprite(spriteInventari);

	spriteSlot = new Sprite(Recursos::obtTextura("seleccio.png"), "Seleccio", glm::vec2(), glm::vec2(7), true);
	spriteSlot->teletransportar(glm::vec2(renderer->width / 2 - 80 * 3.5, renderer->height - spriteSlot->obtTamany().y * 1.7));
	posInicial = spriteSlot->obtPos();
	spriteSlot->indexZ = 3;
	renderer->afegirSprite(spriteSlot);

	for (auto slot : inventari)
	{
		slot->sprite->teletransportar(glm::vec2(renderer->width / 2 - 70 * 4 + 70 * slot->id, renderer->height - 32 - 5));
		renderer->afegirSprite(slot->sprite);
	}
	for (auto vector : inventariGran)
	{
		for (auto slot : vector)
		{
			renderer->afegirSprite(slot->sprite);
		}
	}
}

void Inventari::canviaSeleccionat(const uint8_t seleccio)
{
	if (seleccio >= 0 && seleccio < MAX_ITEMS) {
		slotSeleccionat = seleccio;
		glm::vec2 novaPos = posInicial + glm::vec2(seleccio * 70, 0);
		spriteSlot->teletransportar(novaPos);
	}
}

void Inventari::canviaSelecccionatPer1(int seleccio)
{
	slotSeleccionat += seleccio;
	if (slotSeleccionat < 0) slotSeleccionat = MAX_ITEMS - 1;
	else if (slotSeleccionat >= MAX_ITEMS) slotSeleccionat = 0;
	canviaSeleccionat(slotSeleccionat);
}

Item* Inventari::obtenirItemActual() const
{
	return Recursos::getItem(inventari[slotSeleccionat]->item);
}

void Inventari::afegirItem(int id, uint8_t _quantitat)
{
	if (id == -1) return;
	if (quantitat == MAX_ITEMS) return;
	int i = 0;
	while (i < MAX_ITEMS) {
		Slot* actual = inventari[i];
		if (actual->item == id) {
			actual->quantitat += _quantitat;
			break;
		}
		else if (actual->item == -1) {
			actual->item = id;
			actual->quantitat = _quantitat;
			actual->actualitzaSprite();
			break;
		}
		i++;
	}
	quantitat++;
}

void Inventari::afegirItem(string nom, uint8_t _quantitat)
{
	Item* i = Recursos::getItem(nom);
	if (i == NULL) return;
	afegirItem(i->id,quantitat);
}

void Inventari::alternaVisibilitat()
{
	visible = !visible;
}

void Inventari::render()
{
	if (!visible) return;
	for (auto slot : inventari)
	{
		if (!slot->visible) continue;
		Item* i = Recursos::getItem(slot->item);
		if (i->classe == BLOC) {
			Bloc* b = Recursos::getBloc(i->bloc_id);
			shader->usar();
			glm::mat4 model = slot->sprite->model;
			//model = glm::scale(model, glm::vec3(.75));
			shader->colocarMat4("model", model);

			mapaBlocs->use();
			
			shader->colocarVec2("posicioSprite", glm::vec2(16 * (b->costats % 16), 16 * (int)(b->costats / 16)));
			shader->colocarVec2("tamanyTextura", slot->sprite->textura->obtTamany());

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 12);
			shader->colocarVec4("spriteColor", slot->sprite->color);
			shader->colocarVec2("posicioSprite", glm::vec2(16 * (b->adalt % 16), 16 * (int)(b->adalt / 16)));
			glDrawArrays(GL_TRIANGLES, 12, 6);
			shader->colocarVec4("spriteColor", glm::vec4(1));
			glBindVertexArray(0);
		}
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
		if (Recursos::getBloc(i->bloc_id)->id == GESPA) {
			glm::vec3* color = Recursos::obtColor(Recursos::VERDGESPA);
			sprite->color = glm::vec4(color->r/255,color->g/255,color->b/255, 1.0f);
		}
		else sprite->color = glm::vec4(1.0f);
		sprite->visible = false;
		return;
	}
	else sprite->color = glm::vec4(1.0f);
	uint8_t posSprite = i->sprite;
	sprite->posicioMapa = glm::vec2((posSprite % 16) * 16, (int)(posSprite / 16) * 16);
	sprite->visible = true;
}
