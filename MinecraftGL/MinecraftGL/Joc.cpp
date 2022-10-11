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

void Joc::loop() {
	
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Treure el cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	Chunk2 c;

	int fps = 0;
	float ant = 0.0f;

	float i = 0.0f; float j = 1.0f;

	// El loop del joc, mentre no es tanqui la finestra...
	while (!glfwWindowShouldClose(window))
	{
		camera.moure(deltaTime, window);
		camera.girar(window);

		//cout << "X: "<<camera.obtPos().x << ", Z: " << camera.obtPos().z << endl;

		i += 10;
		j = sin(glfwGetTime());


		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Canvia el color del background
		glClearColor(0.4f, 0.2f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		c.render(); // Nomes renderitzem un quadrat

		// ---- camera ----

		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(i), glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(j,j,j));

		glm::mat4 view = glm::mat4(1.0f);
		// note that we’re translating the scene in the reverse direction
		view = camera.lookAt();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(1600.0f), renderer.aspectRatio(), 0.1f, 100.0f);

		renderer.colocarMat4("model", model);
		renderer.colocarMat4("view", view);
		renderer.colocarMat4("projection", projection);

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

