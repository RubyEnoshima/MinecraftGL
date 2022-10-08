#include "Joc.h"

void Joc::canviarModeMouse(int mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

// Funci� per processar tots els inputs
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

	// Amb F5 es canvia la posici� de la camera
	else if (key == GLFW_KEY_F5 and action == GLFW_PRESS) {
		cout << "Cambiar camara" << endl;
	}

	cout << "Tecla: " << key << endl;
}

int Joc::crearFinestra() {
	int success = renderer.crearFinestra();
	if (success) {
		window = renderer.finestra();
		
		// Funci� per tractar l'input
		glfwSetKeyCallback(window, key_callback);

	}
	return success;
}

void Joc::loop() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Chunk c;
	Camera camera;

	int fps = 0;
	float ant = 0.0f;

	float i = 0.0f;

	// El loop del joc, mentre no es tanqui la finestra...
	while (!glfwWindowShouldClose(window))
	{
		i += 10;
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Canvia el color del background
		glClearColor(0.4f, 0.2f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		c.render(); // Nomes renderitzem un quadrat

		// ---- camera ----
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(i), glm::vec3(1.0f, 1.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f);
		// note that we�re translating the scene in the reverse direction
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(1500.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		renderer.colocarMat4("model", model);
		renderer.colocarMat4("view", view);
		renderer.colocarMat4("projection", projection);

		// -----------------

		glfwSwapBuffers(window); // Volcar l'array de color a la finestra
		glfwPollEvents(); // Processar events

		// Mostrem els frames que hi ha hagut en un segon (fps)
		if (currentFrame-ant>=1.0f) { // Si la difer�ncia �s 1 �s que ha passat un segon
			ant = currentFrame;
			cout << "Fps: " << fps << endl; // Mostrem els frames que hem pogut processar
			fps = 0; // Resetejem el comptador
		}
		fps++;
	}
}

void Joc::gameLoop() {
	cout << "Joc start" << endl;

	if (renderer.carregaShaders()) {
		// El loop de veritat
		loop();

	}

	// Terminar el programa
	glfwTerminate();
}
