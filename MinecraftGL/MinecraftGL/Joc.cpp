#include "Joc.h"
Joc::~Joc() {
	delete mon;
	delete _HUD;
	delete jugador;
}

void Joc::canviarModeMouse(int mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Joc::canviarProjeccio() {
	jugador->obtCamera()->setProjection(glm::radians(3000.0f), renderer.aspectRatio(), 0.1f, 1000.0f);
	renderer.colocarMat4("projection", jugador->obtCamera()->getProjection());
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));

	joc->canviarProjeccio();
}

// Funció per processar tots els inputs
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
		{
			if (key == GLFW_KEY_9) joc->tipusCub = TULIPA_TARONJA;
			else joc->tipusCub = key%48;
			return;
		}
		switch (key) {
			case GLFW_KEY_LEFT_CONTROL:
				joc->jugador->correr();
				break;
			case GLFW_KEY_W: case GLFW_KEY_A: case GLFW_KEY_S: case GLFW_KEY_D: case GLFW_KEY_SPACE: case GLFW_KEY_LEFT_SHIFT:
				//joc->jugador->moure(joc->deltaTime, key);
				joc->tecles[key] = true;
				break;
			// ESC: es tanca la finestra i tanca el joc
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
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
			// F2 = Cull/!Cull
			case GLFW_KEY_F2:
				joc->Culling();
				break;
			// F4: 
			case GLFW_KEY_F4:
				joc->VSync();
				break;
			// F5: es canvia la posició de la camera
			case GLFW_KEY_F5:
				cout << "Cambiar camara" << endl;
				break;
			case GLFW_KEY_F1:
				joc->_HUD->alternaVisibilitat();
				break;
		}

	}
	else if (action == GLFW_RELEASE) {
		switch (key)
		{
			case GLFW_KEY_W: case GLFW_KEY_A: case GLFW_KEY_S: case GLFW_KEY_D: case GLFW_KEY_SPACE: case GLFW_KEY_LEFT_SHIFT:
				//joc->jugador->moure(joc->deltaTime, key);
				joc->tecles[key] = false;
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
	}
	else {
		glEnable(GL_CULL_FACE);
	}
}

void Joc::VSync() {
	_VSync = !_VSync;
	glfwSwapInterval(_VSync);

}

void Joc::moure()
{
	for (auto tecla : tecles) {
		if (tecla.second) {
			jugador->moure(deltaTime, tecla.first);
		}
	}
}

int Joc::crearFinestra() {
	int success = renderer.crearFinestra();
	if (success) {
		window = renderer.finestra();
		
		// Funció per tractar l'input
		glfwSetKeyCallback(window, key_callback);

	}
	return success;
}

void Joc::ObtenirCubMira() {	
	// Fent us de raycast
	//Ray r;
	//r.origen = jugador->obtCamera()->obtPos() + jugador->obtCamera()->obtDireccio();
	///*r.origen = glm::vec3((int)(r.origen.x), (int)(r.origen.y), (int)(r.origen.z));
	//cout << r.origen << endl;*/
	//r.direccion = jugador->obtCamera()->obtDireccio();
	//int i = 0; int passes = 10;
	//while (mon->obtenirCub(r.origen.x, r.origen.y, r.origen.z) == AIRE && i < passes) { r.origen += jugador->obtCamera()->obtDireccio(); i++; }

	//CubActual = glm::vec3((int)r.origen.x, (int)r.origen.y, (int)r.origen.z);
	//cout << CubActual << jugador->obtCamera()->obtPos() << endl;
	//return;

	// Desprojectant coordenades de pantalla
	int ww = renderer.obtenirTamany().first;
	int wh = renderer.obtenirTamany().second;
	float depth;
	glReadPixels(ww / 2, wh / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	if (depth <= 0 || depth >= 1) {
		CubActual = glm::vec3(-1, -1, -1);
		return;
	}

	glm::vec4 viewport = glm::vec4(0, 0, ww, wh);
	glm::vec3 wincoord = glm::vec3(ww / 2, wh / 2, depth);
	glm::vec3 objcoord = glm::unProject(wincoord, jugador->obtCamera()->getView(), jugador->obtCamera()->getProjection(), viewport);
	CubActual = glm::vec3(floorf(objcoord.x), floorf(objcoord.y), floorf(objcoord.z));
	
	//cout << CubActual << endl;
}

void Joc::DestruirCub() {
	// Si és un cub vàlid
	if (CubActual.x != -1 && CubActual.y != -1 && CubActual.z != -1)
		mon->canviarCub(CubActual.x, CubActual.y, CubActual.z, 0, true, true);
}

glm::vec3 Joc::ObtenirCostat() {

	// Renderitzem només el cub que estem mirant d'una manera especial
	renderer.DibuixarDarrera();
	glClearColor(rgb(0), rgb(0), rgb(0), 1.0f);

	GLenum err;
	while ((err = glGetError()) == GL_INVALID_OPERATION) {
		cerr << "OpenGL error: " << err << endl;
		cerr << glfwGetVersionString();
	}

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

	int ww = renderer.obtenirTamany().first;
	int wh = renderer.obtenirTamany().second;
	// Llegim el color del costat que estem mirant
	glm::vec3 color;
	glReadPixels(ww / 2, wh / 2, 1, 1, GL_RGB, GL_FLOAT, &color);
	
	// Tornem a posar el buffer i el shader per defecte i aixi dibuixem l'escena tal qual
	renderer.DibuixarFront();
	

	// Mirem quin costat és pel color que hem obtingut abans
	if (color[0] > 0) {
		if (color[2] > 0) return glm::vec3(0,0,-1);
		if (color[1] > 0) return glm::vec3(0,-1,0);
		return glm::vec3(0, 0, 1);
	}
	if (color[1] > 0) {
		if (color[2] > 0) return glm::vec3(-1,0,0);
		return glm::vec3(0,1,0);
	}
	if (color[2] > 0) return glm::vec3(1, 0, 0);
	return glm::vec3(-1, -1, -1);
}

void Joc::PosarCub() {
	// Si és un cub vàlid
	if (CubActual.x == -1 || CubActual.y == -1 || CubActual.z == -1 ) return;
	

	// Obtenim el costat al que estem mirant
	glm::vec3 Costat = ObtenirCostat();
	if (Costat.x==-1 && Costat.y==-1) return;
	
	// Canviem el cub
	mon->canviarCub(CubActual.x + Costat.x, CubActual.y + Costat.y, CubActual.z + Costat.z, tipusCub, false, true);

}

void mouse_click_callback(GLFWwindow* window, int click, int action, int mods) {

	if (click == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));

		joc->DestruirCub();
	}

	else if (click == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));

		joc->PosarCub();
	}
}

void Joc::loop() {
	// Matriu model
	glm::mat4 model = glm::mat4(1.0f);
	jugador->obtCamera()->setModel(model);
	renderer.colocarMat4("model", model);
	/*model = glm::rotate(model, glm::radians(i), glm::vec3(1.0f, 1.0f, 1.0f));
	  model = glm::scale(model, glm::vec3(j,j,j));*/

	// Matriu de projecció
	canviarProjeccio();
	
	// Matriu view
	glm::mat4 view = glm::mat4(1.0f);

	// Per controlar els fps
	int fps = 0;
	float ant = 0.0f;

	renderer.canviarColorLlum(glm::vec3(0.75f, 0.75f, 0.75f));
	glm::vec3 pos = glm::vec3(0, Y, 0.0f);

	

	renderer.activaBounding(0);

	// El loop del joc, mentre no es tanqui la finestra...
	while (!glfwWindowShouldClose(window))
	{
		//jugador->obtCamera()->moure(deltaTime, window);
		jugador->obtCamera()->girar(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Canvia el color del fons
		glClearColor(rgb(110), rgb(170), rgb(255), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//renderer.canviarColor(glm::vec4(rgb(255), rgb(255), rgb(255), 1.0f));
		mon->render();
		renderer.canviarPosLlum(pos);

		glfwPollEvents(); // Processar events
		moure();

		// Obtenim el cub al que estem mirant i el senyalem al mon
		ObtenirCubMira();
		if(mon->obtenirCub(CubActual.x,CubActual.y,CubActual.z)>0)
			mon->BoundingBox(CubActual.x, CubActual.y, CubActual.z);


		view = jugador->obtCamera()->lookAt();
		renderer.colocarMat4("view", view);

		_HUD->render();

		glfwSwapBuffers(window); // Volcar l'array de color a la finestra

		// Mostrem els frames que hi ha hagut en un segon (fps)
		if (currentFrame-ant>=1.0f) { // Si la diferència és 1 és que ha passat un segon
			ant = currentFrame;
			//cout << "Fps: " << fps << endl; // Mostrem els frames que hem pogut processar
			glfwSetWindowTitle(window, ("MinecraftGL - FPS: " + to_string(fps)).c_str());
			fps = 0; // Resetejem el comptador

		}
		fps++;
	}
}

void Joc::gameLoop() {
	cout << "Joc start\n\n\n";

	if (renderer.carregaShaders()) {
		cout << "-------------\n\n\n\n";

		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
		glfwSetMouseButtonCallback(window, mouse_click_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Treure el cursor
		canviarModeMouse(GLFW_CURSOR_DISABLED);

		mon = new SuperChunk(&renderer);
		_HUD = new HUD(&renderer);
		jugador = new Jugador(new Camera());

		// Posem el color taronja
		//renderer.canviarColor(glm::vec4(rgb(255), rgb(148), rgb(73), 1.0f));

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 0);

		glfwSwapInterval(_VSync);

		// El loop de veritat
		loop();

	}

	// Terminar el programa
	glfwTerminate();
}

