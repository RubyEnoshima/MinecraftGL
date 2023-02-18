#include "Joc.h"
Joc::~Joc() {
	delete mon;
}

void Joc::canviarModeMouse(int mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Joc::canviarProjeccio() {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(3000.0f), renderer.aspectRatio(), 0.1f, 100.0f);
	camera.setProjection(projection);
	renderer.colocarMat4("projection", projection);
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

	// Si es pressiona ESC, es tanca la finestra
	if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Amb F1 es canvia entre mode normal i wireframe
	else if (key == GLFW_KEY_F1 and action == GLFW_PRESS) {
		int mode;
		glGetIntegerv(GL_POLYGON_MODE, &mode);
		if (mode == GL_FILL) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	// Amb F5 es canvia la posició de la camera
	else if (key == GLFW_KEY_F5 and action == GLFW_PRESS) {
		cout << "Cambiar camara" << endl;
	}

	// F2 = Cull/!Cull
	else if (key == GLFW_KEY_F2 and action == GLFW_PRESS) {
		
		joc->Culling();
	}

	// F4
	else if (key == GLFW_KEY_F4 and action == GLFW_PRESS) {
		joc->VSync();
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
	glm::vec3 objcoord = glm::unProject(wincoord, camera.getView(), camera.getProjection(), viewport);
	CubActual = glm::vec3(floorf(objcoord.x), floorf(objcoord.y) + Y / 2, floorf(objcoord.z));
	
	//cout << CubActual << endl;
}

void Joc::DestruirCub() {
	// Si és un cub vàlid
	if (CubActual.x != -1 && CubActual.y != -1 && CubActual.z != -1)
		mon->canviarCub(CubActual.x, CubActual.y, CubActual.z, 0);
}

glm::vec3 Joc::ObtenirCostat() {

	// Renderitzem només el cub que estem mirant d'una manera especial
	renderer.DibuixarDarrera();

	GLenum err;
	while ((err = glGetError()) == GL_INVALID_OPERATION) {
		cerr << "OpenGL error: " << err << endl;
		cerr << glfwGetVersionString();
	}

	// Canviem el shader que fem servir per un mes senzill
	renderer.usarShader(1);
	renderer.colocarMat4("view", camera.lookAt());
	renderer.colocarMat4("projection", camera.getProjection());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mon->renderCub(CubActual.x, CubActual.y, CubActual.z);

	int ww = renderer.obtenirTamany().first;
	int wh = renderer.obtenirTamany().second;

	// Llegim el color del costat que estem mirant
	glm::vec3 color;
	glReadPixels(ww / 2, wh / 2, 1, 1, GL_RGB, GL_FLOAT, &color);
	
	// Tornem a posar el buffer i el shader per defecte i aixi dibuixem l'escena tal qual
	renderer.usarShader(0);
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
	return glm::vec3(1, 0, 0);
}

void Joc::PosarCub() {
	// Si és un cub vàlid
	if (CubActual.x == -1 && CubActual.y == -1 && CubActual.z == -1) return;

	// Obtenim el costat al que estem mirant
	glm::vec3 Costat = ObtenirCostat();
		
	// Canviem el cub
	mon->canviarCub(CubActual.x + Costat.x, CubActual.y + Costat.y, CubActual.z + Costat.z, LLUM);
		
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
	camera.setModel(model);
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

	// El loop del joc, mentre no es tanqui la finestra...
	while (!glfwWindowShouldClose(window))
	{
		camera.moure(deltaTime, window);
		camera.girar(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Canvia el color del background
		glClearColor(rgb(110), rgb(170), rgb(255), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//renderer.canviarColor(glm::vec4(rgb(255), rgb(255), rgb(255), 1.0f));
		mon->render();
		//mon->renderCub(7, 62, 4);
		renderer.canviarPosLlum(pos);

		glfwPollEvents(); // Processar events

		// Obtenim el cub al que estem mirant i el senyalem al mon
		ObtenirCubMira();
		
		//cout << CubActual << endl;
		//renderer.canviarColor(glm::vec4(rgb(0), rgb(0), rgb(0), 1.0f));
		if(mon->obtenirCub(CubActual.x,CubActual.y,CubActual.z)>0)
			mon->BoundingBox(CubActual.x, CubActual.y, CubActual.z);


		view = camera.lookAt();
		renderer.colocarMat4("view", view);


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

		//SuperChunk sp(&renderer);
		mon = new SuperChunk(&renderer);

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

