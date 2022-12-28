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

glm::vec3 calcularNormal(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2)
{
	glm::vec3 A = P1 - P0, B = P2 - P1;
	glm::vec3 N(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
	return glm::normalize(N);
}

void afegirVertex(vector<glm::vec3>& vertices, int8_t x, int8_t y, int8_t z) {
	// Posicio
	vertices.push_back(glm::vec3(x,y,z));
}

void obtenirCares(vector<glm::vec3>& vertices, int8_t x, int8_t y, int8_t z) {
	// Cara esq
	{
		afegirVertex(vertices, x, y, z);
		afegirVertex(vertices, x, y, z + 1);
		afegirVertex(vertices, x, y + 1, z);
		//afegirVertex(vertices, x, y + 1, z);
		//afegirVertex(vertices, x, y, z + 1);
		//afegirVertex(vertices, x, y + 1, z + 1);

	}

	// Cara dre
	{
		afegirVertex(vertices, x + 1, y, z);
		afegirVertex(vertices, x + 1, y + 1, z);
		afegirVertex(vertices, x + 1, y, z + 1);
		//afegirVertex(vertices, x + 1, y + 1, z);
		//afegirVertex(vertices, x + 1, y + 1, z + 1);
		//afegirVertex(vertices, x + 1, y, z + 1);
	}

	// Cara frontal
	{
		afegirVertex(vertices, x, y, z + 1);
		afegirVertex(vertices, x + 1, y, z + 1);
		afegirVertex(vertices, x, y + 1, z + 1);

		//afegirVertex(vertices, x, y + 1, z + 1);
		//afegirVertex(vertices, x + 1, y, z + 1);
		//afegirVertex(vertices, x + 1, y + 1, z + 1);

	}

	// Cara darrera
	{
		afegirVertex(vertices, x, y, z);
		afegirVertex(vertices, x, y + 1, z);
		afegirVertex(vertices, x + 1, y, z);

		//afegirVertex(vertices, x, y + 1, z);
		//afegirVertex(vertices, x + 1, y + 1, z);
		//afegirVertex(vertices, x + 1, y, z);

	}

	// Cara adalt
	{
		afegirVertex(vertices, x, y + 1, z);
		afegirVertex(vertices, x, y + 1, z + 1);
		afegirVertex(vertices, x + 1, y + 1, z);
		//afegirVertex(vertices, x + 1, y + 1, z);
		//afegirVertex(vertices, x, y + 1, z + 1);
		//afegirVertex(vertices, x + 1, y + 1, z + 1);

	}

	// Cara sota
	{
		afegirVertex(vertices, x, y, z);
		afegirVertex(vertices, x + 1, y, z);
		afegirVertex(vertices, x, y, z + 1);
		//afegirVertex(vertices, x + 1, y, z);
		//afegirVertex(vertices, x + 1, y, z + 1);
		//afegirVertex(vertices, x, y, z + 1);

	}

}

void Linea(int8_t x, int8_t y, int8_t z)
{
	// Cube 1x1x1, centered on origin
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
}

void Joc::ObtenirCubMira() {
	int ww = renderer.obtenirTamany().first;
	int wh = renderer.obtenirTamany().second;
	float depth;
	glReadPixels(ww / 2, wh / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	if (depth <= 0 || depth >= 1) {
		CubActual = glm::vec3(X, Y, Z);
		return;
	}

	glm::vec4 viewport = glm::vec4(0, 0, ww, wh);
	glm::vec3 wincoord = glm::vec3(ww / 2, wh / 2, depth);
	glm::vec3 objcoord = glm::unProject(wincoord, camera.getView(), camera.getProjection(), viewport);
	CubActual = glm::vec3(floorf(objcoord.x) + 8, floorf(objcoord.y) + Y / 2, floorf(objcoord.z) + 8);
	
	
}

void Joc::DestruirCub() {
	if (CubActual.x != X && CubActual.y != Y && CubActual.z != Z)
		mon->canviarCub(CubActual.x, CubActual.y, CubActual.z, 0);
}

void Joc::PosarCub() {
	if (CubActual.x != X && CubActual.y != Y && CubActual.z != Z) {

		vector<glm::vec3> vertices;
		obtenirCares(vertices, CubActual.x, CubActual.y, CubActual.z);

		RayCast raycast(&camera, &renderer);
		glm::vec3 ray = raycast.calcularRay();

		int i = 0; // 0: esq, 1: dre, 2: front, 3: darr, 4: adalt, 5: sota
		int j = 0;
		while (i < 6 && !raycast.intersecta(vertices[j], vertices[j + 1], vertices[j + 2])) {
			i++;
			j += 3;
		}
		cout << i << endl;

		mon->canviarCub(CubActual.x, CubActual.y, CubActual.z, 2);
		
	}
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
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(3000.0f), renderer.aspectRatio(), 0.1f, 100.0f);
	camera.setProjection(projection);
	renderer.colocarMat4("projection", projection);

	// Matriu view
	glm::mat4 view = glm::mat4(1.0f);

	// Per controlar els fps
	int fps = 0;
	float ant = 0.0f;

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
		
		renderer.canviarColor(glm::vec4(rgb(255), rgb(255), rgb(255), 1.0f));
		mon->render();

		// Obtenim el cub al que estem mirant i el senyalem al mon
		ObtenirCubMira();
		//cout << CubActual << endl;
		renderer.canviarColor(glm::vec4(rgb(0), rgb(0), rgb(0), 1.0f));
		mon->BoundingBox(CubActual.x, CubActual.y, CubActual.z);


		view = camera.lookAt();
		renderer.colocarMat4("view", view);


		glfwSwapBuffers(window); // Volcar l'array de color a la finestra
		glfwPollEvents(); // Processar events

		// Mostrem els frames que hi ha hagut en un segon (fps)
		if (currentFrame-ant>=1.0f) { // Si la diferència és 1 és que ha passat un segon
			ant = currentFrame;
			//cout << "Fps: " << fps << endl; // Mostrem els frames que hem pogut processar
			fps = 0; // Resetejem el comptador

		}
		fps++;
	}
}

void Joc::gameLoop() {
	cout << "Joc start" << endl << endl;

	if (renderer.carregaShaders()) {
		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
		glfwSetMouseButtonCallback(window, mouse_click_callback);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Treure el cursor
		canviarModeMouse(GLFW_CURSOR_DISABLED);

		//SuperChunk sp(&renderer);
		mon = new SuperChunk(&renderer);

		// Posem el color taronja
		renderer.canviarColor(glm::vec4(rgb(255), rgb(148), rgb(73), 1.0f));

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 0);
		// El loop de veritat
		loop();

	}

	// Terminar el programa
	glfwTerminate();
}

