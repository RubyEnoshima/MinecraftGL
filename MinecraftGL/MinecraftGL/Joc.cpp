#include "Joc.h"

void Joc::canviarModeMouse(int mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

// Funció per processar tots els inputs
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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

	else if (key == GLFW_KEY_F2 and action == GLFW_PRESS) {
		if (glIsEnabled(GL_CULL_FACE)) {
			glDisable(GL_CULL_FACE);
		}
		else {
			glEnable(GL_CULL_FACE);
		}
	}

	// cout << "Tecla: " << key << endl;
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

void mouse_click_callback(GLFWwindow* window, int click, int action, int mods) {
	if (click == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Joc* joc = reinterpret_cast<Joc*>(glfwGetWindowUserPointer(window));
		
		RayCast r(&joc->camera, &joc->renderer, window);
		glm::vec3 ray = r.calcularRay();
		glm::vec3 O = joc->camera.obtPos();
		float t;
		cout << "Ray: " << ray.x+O.x << " " << ray.y+O.y << " " << ray.z+O.z << endl;
		//cout << r.rayTriangleIntersect(O,ray,glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), t);
		
		float xCentre = joc->renderer.obtenirCentre().first;
		float yCentre = joc->renderer.obtenirCentre().second;
		float px[4];
		glReadPixels(1920/2, 1080/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &px);
		glm::vec3 kaka = glm::normalize(glm::unProject(glm::vec3(xCentre,yCentre,1.0f), joc->camera.getView() * joc->camera.getModel(), joc->camera.getProjection(), glm::vec4(0.0f,0.0f,800.0f,600.0f)));
		cout << "x: " << kaka[0] << " y: " << kaka[1] << " z: " << kaka[2] << endl;
		
	}
}

void Joc::loop() {
	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
	glfwSetMouseButtonCallback(window, mouse_click_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Treure el cursor
	canviarModeMouse(GLFW_CURSOR_DISABLED);
	
	SuperChunk sp(&renderer);

	int fps = 0;
	float ant = 0.0f;

	float i = 0.0f; float j = 1.0f;

	renderer.canviarColor(glm::vec4(rgb(255), rgb(148), rgb(73), 1.0f));


	glm::mat4 model = glm::mat4(1.0f);
	camera.setModel(model);
	renderer.colocarMat4("model", model);
	//model = glm::rotate(model, glm::radians(i), glm::vec3(1.0f, 1.0f, 1.0f));
	//model = glm::scale(model, glm::vec3(j,j,j));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(1600.0f), renderer.aspectRatio(), 0.1f, 100.0f);
	camera.setProjection(projection);
	renderer.colocarMat4("projection", projection);

	glm::mat4 view = glm::mat4(1.0f);


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
		

		sp.render();

		// ---- camera ----

		view = camera.lookAt();
		renderer.colocarMat4("view", view);
		

		// -----------------

		glfwSwapBuffers(window); // Volcar l'array de color a la finestra
		glfwPollEvents(); // Processar events

		// Mostrem els frames que hi ha hagut en un segon (fps)
		if (currentFrame-ant>=1.0f) { // Si la diferència és 1 és que ha passat un segon
			ant = currentFrame;
			cout << "Fps: " << fps << endl; // Mostrem els frames que hem pogut processar
			fps = 0; // Resetejem el comptador

		}
		fps++;
	}
}

void Joc::gameLoop() {
	cout << "Joc start" << endl << endl;

	if (renderer.carregaShaders()) {
		// El loop de veritat
		loop();

	}

	// Terminar el programa
	glfwTerminate();
}

