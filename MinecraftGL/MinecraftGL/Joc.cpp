#include "Joc.h"
Joc::~Joc() {
	delete mon;
	delete _HUD;
	delete jugador;
	delete nuvols;
}

glm::vec2 Joc::obtMousePos() const
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return glm::vec2(xpos,ypos);
}

void Joc::canviarModeMouse(int mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Joc::canviarProjeccio() {
	Camera* c = jugador->obtCamera();
	c->setProjection(renderer.aspectRatio());
	renderer.colocarMat4("projection", c->getProjection());
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));

	joc->canviarProjeccio();
	Recursos::width = width;
	Recursos::height = height;
}

// Funci� per processar tots els inputs
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9)
		{
			uint8_t num = key % 48;
			joc->jugador->inventari->canviaSeleccionat(num-1);
			return;
		}
		switch (key) {
			case GLFW_KEY_LEFT_CONTROL:
				joc->jugador->correr();
				break;
			// En cas de voler moure's, ens guardem l'estat de les tecles
			case GLFW_KEY_W: case GLFW_KEY_A: case GLFW_KEY_S: case GLFW_KEY_D: case GLFW_KEY_SPACE: case GLFW_KEY_LEFT_SHIFT:
				joc->tecles[key] = true;
				break;
			// ESC: es tanca la finestra i tanca el joc si no es a dins d'un inventari
			case GLFW_KEY_ESCAPE:
				if (joc->modeInventari) {
					joc->Inventari();
				}
				else {
					Recursos::jocAcabat = true;
					glfwSetWindowShouldClose(window, true);

				}
				break;
			// C: canvia mode espectador <-> creatiu
			case GLFW_KEY_C:
				joc->CanviarMode();
				break;
			// E: obrir inventari
			case GLFW_KEY_E:
				joc->Inventari();
				break;
			
			// F1: oculta HUD
			case GLFW_KEY_F1:
				joc->_HUD->alternaVisibilitat();
				joc->jugador->inventari->alternaVisibilitat();
				break;
			// F2 = Cull/!Cull
			case GLFW_KEY_F2:
				joc->Culling();
				break;
			// F3 = mostra text debug amb informaci�
			case GLFW_KEY_F3:
				joc->HUDDebug();
				break;
			// F4: VSync
			case GLFW_KEY_F4:
				joc->VSync();
				break;
			// F5: es canvia la posici� de la camera
			case GLFW_KEY_F5:
				cout << "Cambiar camara" << endl;
				break;
			// F6: frustum
			case GLFW_KEY_F6:
				joc->Frustum();
				break;
			case GLFW_KEY_F7:
				joc->CanviaHora();
				break;
			// F10: es canvia entre mode normal i wireframe
			case GLFW_KEY_F10:
				int mode;
				glGetIntegerv(GL_POLYGON_MODE, &mode);
				if (mode == GL_FILL) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				break;
		}

	}
	else if (action == GLFW_RELEASE) {
		switch (key)
		{
			case GLFW_KEY_W: case GLFW_KEY_A: case GLFW_KEY_S: case GLFW_KEY_D: case GLFW_KEY_SPACE: case GLFW_KEY_LEFT_SHIFT:
				joc->tecles[key] = false;
				joc->jugador->parar();
				break;
			case GLFW_KEY_LEFT_CONTROL:
				joc->jugador->caminar();
				break;
			default:
				break;
		}
	}
}

void Joc::Culling() {
	_Culling = !_Culling;
	if (!_Culling) {
		glDisable(GL_CULL_FACE);
		cout << "Culling desactivat" << endl;
	}
	else {
		glEnable(GL_CULL_FACE);
		cout << "Culling activat" << endl;

	}
}

void Joc::VSync() {
	_VSync = !_VSync;
	glfwSwapInterval(_VSync);

}

void Joc::Frustum()
{
	mon->activaFrustum = !mon->activaFrustum;
	if (mon->activaFrustum) {
		cout << "Frustum Culling activat" << endl;
	}
	else {
		cout << "Frustum Culling desactivat" << endl;

	}
}

void Joc::HUDDebug()
{
	_HUD->modeDebug();
}

void Joc::Inventari()
{
	modeInventari = !modeInventari;
	int mode = GLFW_CURSOR_DISABLED;
	if (modeInventari) {
		mode = GLFW_CURSOR_NORMAL;
		jugador->parar();
	}
	canviarModeMouse(mode);
	glfwSetCursorPos(window, Recursos::width/2, Recursos::height/2);
	jugador->inventari->obrir();
}

void Joc::CanviaHora()
{
	nit = !nit;
	renderer.activaNit(nit);
	nuvols->canviaHora(nit);
}

void Joc::moure()
{
	jugador->moure(tecles);
}

void Joc::CanviarMode()
{
	if (mode == ESPECTADOR) mode = CREATIU;
	else mode = ESPECTADOR;
	jugador->canviaMode(mode);
}

int Joc::crearFinestra() {
	int success = renderer.crearFinestra();
	if (success > 0) {
		window = renderer.finestra();
	}
	return success;
}

void Joc::ObtenirCubMira() {	
	// Fent us de raycast
	//Ray r;
	//r.origen = jugador->obtCamera()->obtPos();// +jugador->obtCamera()->obtDireccio();
	//r.origen = glm::vec3(floor(r.origen.x+0.5), floor(r.origen.y+0.5), floor(r.origen.z+0.5));
	//r.direccion = jugador->obtCamera()->obtDireccio();
	//int i = 0; int passes = 10;
	//while (mon->obtenirCub(r.origen.x, r.origen.y, r.origen.z) == AIRE && i < passes) { r.origen += jugador->obtCamera()->obtDireccio(); i++; }
	//CubActual = glm::vec3(floor(r.origen.x), floor(r.origen.y), floor(r.origen.z));
	//return;

	// Desprojectant coordenades de pantalla
	int ww = Recursos::width;
	int wh = Recursos::height;
	float depth;
	glReadPixels(ww / 2, wh / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth); // Llegim la profunditat del pixel al que estem mirant
	if (depth <= 0 || depth >= 1) {
		CubActual = glm::vec3(-1, -1, -1);
		return;
	}
	
	glm::vec4 viewport = glm::vec4(0, 0, ww, wh);
	glm::vec3 wincoord = glm::vec3(ww / 2, wh / 2, depth);
	glm::vec3 objcoord = glm::unProject(wincoord, jugador->obtCamera()->getView(), jugador->obtCamera()->getProjection(), viewport);
	CubActual = glm::vec3(floorf(objcoord.x), floorf(objcoord.y), floorf(objcoord.z));
	//if((mon->obtenirCub(CubActual)) == AIGUA) CubActual = glm::vec3(-1, -1, -1);

}

void Joc::DestruirCub() {
	// Si �s un cub v�lid
	if (CubActual.y != -1)
		mon->canviarCub(CubActual.x, CubActual.y, CubActual.z, AIRE, true, true);
}

glm::vec3 Joc::ObtenirCostat() {

	// Renderitzem només el cub que estem mirant d'una manera especial
	renderer.DibuixarDarrera();
	glClearColor(rgb(0), rgb(0), rgb(0), 1.0f);

	// Canviem el shader que fem servir per un mes senzill
	renderer.usarShader(1);
	renderer.colocarMat4("view", jugador->obtCamera()->lookAt());
	renderer.colocarMat4("projection", jugador->obtCamera()->getProjection());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bool renderitzat = mon->renderCub(CubActual.x, CubActual.y, CubActual.z);
	if (!renderitzat) {
		renderer.DibuixarFront();
		return glm::vec3(-1,-1,-1);
	}

	int ww = Recursos::width;
	int wh = Recursos::height;
	// Llegim el color del costat que estem mirant
	glm::vec3 color;
	glReadPixels(ww / 2, wh / 2, 1, 1, GL_RGB, GL_FLOAT, &color);
	
	// Tornem a posar el buffer i el shader per defecte i aixi dibuixem l'escena tal qual
	renderer.DibuixarFront();

	// Mirem quin costat és pel color que hem obtingut abans
	auto it = colorsCostat.find(color);
	if (it != colorsCostat.end()) return it->second;
	return glm::vec3(-1);
}

void Joc::PosarCub(uint8_t tipus) {
	// Si és un cub vàlid
	if (CubActual.y == -1) return;

	// Obtenim el costat al que estem mirant
	glm::vec3 Costat = ObtenirCostat();
	if (Costat.x==-1 && Costat.y==-1) return;
	
	glm::vec3 posNova = CubActual + Costat;
	if (jugador->obtPosBloc() == posNova || jugador->obtPosBloc(false) == posNova) return;

	// Canviem el cub
	mon->canviarCub(posNova.x, posNova.y, posNova.z, tipus, true, true);

}

void Joc::Usar()
{
	Item* item = jugador->inventari->obtenirItemActual();
	if (item == NULL) return;

	if (item->classe == BLOC) {
		PosarCub(item->id);
	}
	else {
		cout << "Encara no esta fet..." << endl;
	}
}

void mouse_click_callback(GLFWwindow* window, int click, int action, int mods) {

	Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));
	if (click == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

		if (!joc->modeInventari) joc->DestruirCub();
		else joc->jugador->inventari->agafarItem();
	}

	else if (click == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (!joc->modeInventari) joc->Usar();
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));
	joc->jugador->inventari->canviaSelecccionatPer1(-yoffset);
}

void Joc::loop() {
	// Matriu model
	glm::mat4 model = glm::mat4(1.0f);
	jugador->obtCamera()->setModel(model);
	renderer.colocarMat4("model", model);

	// Matriu de projecci�
	canviarProjeccio();
	
	// Matriu view
	glm::mat4 view = glm::mat4(1.0f);

	// Per controlar els fps
	int fps = 0;
	float ant = 0.0f;

	nuvols = new Nuvols(jugador->obtCamera()->getProjection());
	nuvols->canviaHora(nit);

	// Fem que un thread s'encarregui de la gesti� de chunks
	thread t([&]() {
		while (!glfwWindowShouldClose(window)) {
			mon->comprovarChunks(jugador->chunkActual());
			mon->descarregarChunks();
			mon->carregarChunks();
			
		}
	});

	// En un thread apart fem que s'actualitzi el m�n
	thread t2([&]() {
		while (!glfwWindowShouldClose(window)) { 
			mon->update(jugador->chunkActual()); 
		} 
	});

	const glm::vec3* colorCelDia = Recursos::obtColor(Recursos::CEL);
	const glm::vec3* colorCelNit = Recursos::obtColor(Recursos::CEL_NIT);
	renderer.activaBounding(0);
	// El loop del joc, mentre no es tanqui la finestra...
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if(!modeInventari) jugador->obtCamera()->girar(window);
		
		jugador->update(deltaTime, mon->obtenirAABB(jugador->obtPosBloc()));

		// Canvia el color del fons
		if(!nit) glClearColor(rgb(colorCelDia->x), rgb(colorCelDia->y), rgb(colorCelDia->z), 1.0f);
		else glClearColor(rgb(colorCelNit->x), rgb(colorCelNit->y), rgb(colorCelNit->z), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		nuvols->update(jugador->obtPos2D(),deltaTime);
		nuvols->render(view);

		bool sotaAigua = mon->obtenirCub(jugador->obtPosBloc(false)) == AIGUA;
		mon->render(&(jugador->obtCamera()->frustum));

		glfwPollEvents(); // Processar events

		if (!modeInventari) {
			moure();
			// Obtenim el cub al que estem mirant i el senyalem al mon
			ObtenirCubMira();
			if(CubActual.y >= 0 && mon->obtenirCub(CubActual.x,CubActual.y,CubActual.z)>0)
				mon->BoundingBox(CubActual.x, CubActual.y, CubActual.z);

		}

		mon->render(&(jugador->obtCamera()->frustum), true, sotaAigua);

		renderer.activaAigua(sotaAigua);

		view = jugador->obtCamera()->lookAt();
		renderer.colocarMat4("view", view);

		jugador->inventari->render(obtMousePos());
		_HUD->render(jugador->obtPosBloc(), CubActual);

		glfwSwapBuffers(window); // Volcar l'array de color a la finestra

		// Mostrem els frames que hi ha hagut en un segon (fps)
		if (currentFrame-ant>=1.0f) { // Si la difer�ncia �s 1 �s que ha passat un segon
			ant = currentFrame;
			// Mostrem els frames que hem pogut processar
			glfwSetWindowTitle(window, ("MinecraftGL - FPS: " + to_string(fps)).c_str());
			fps = 0; // Resetejem el comptador
			
		}

		fps++;
	}

	t.join();
	t2.join();
}

void Joc::start() {
	cout << "Joc start\n\n\n";

	if (renderer.carregaShaders()) {
		cout << "-------------\n\n\n\n";

		// Funcions de GLFW per contexte
		glfwSetKeyCallback(window, key_callback);
		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
		glfwSetMouseButtonCallback(window, mouse_click_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Treure el cursor
		canviarModeMouse(GLFW_CURSOR_DISABLED);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 0);

		glfwSwapInterval(_VSync);

		mon = new SuperChunk(&renderer);
		jugador = new Jugador(new Camera(), renderer.obtShader());
		_HUD = new HUD(jugador->inventari);

		// El loop de veritat
		loop();

	}

	// Terminar el programa
	glfwTerminate();
}

