#include "Joc.h"

int Joc::carregaShaders() {
	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	ifstream vertexShaderStream(vertexShaderSource); // Obrim l'arxiu

	if (!vertexShaderStream.is_open()) return -1; // Si no es pot obrir, sortim de la funció

	// Llegim el codi en un sol string
	string vertexShaderCodi;
	stringstream sstr;
	sstr << vertexShaderStream.rdbuf();
	vertexShaderCodi = sstr.str();

	vertexShaderStream.close(); // Tanquem l'arxiu

	// Compilem el Vertex Shader
	cout << "Compilant Vertex Shader..." << endl;
	char const* vertexShaderPunter = vertexShaderCodi.c_str(); // Per compilar no es permeten strings
	glShaderSource(vertexShader, 1, &vertexShaderPunter, NULL);
	glCompileShader(vertexShader);

	// Comprovem si hi ha errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR: No s'ha pogut compilar el Vertex Shader\n" << infoLog << endl;
		return -1;
	}

	cout << "Vertex Shader compilat" << endl << endl;

	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	ifstream fragmentShaderStream(fragmentShaderSource); // Obrim l'arxiu

	if (!fragmentShaderStream.is_open()) return -1; // Si no es pot obrir, sortim de la funció

	// Llegim el codi en un sol string
	string fragmentShaderCodi;
	sstr.str(""); // Netejem l'stream perquè no es concateni amb l'anterior vertex shader
	sstr << fragmentShaderStream.rdbuf();
	fragmentShaderCodi = sstr.str();

	fragmentShaderStream.close(); // Tanquem l'arxiu

	// Compilem el Fragment Shader
	cout << "Compilant Fragment Shader..." << endl;
	char const* fragmentShaderPunter = fragmentShaderCodi.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderPunter, NULL);
	glCompileShader(fragmentShader);

	// Comprovem si hi ha errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR: No s'ha pogut compilar el Fragment Shader\n" << infoLog << endl;
		return -1;
	}

	// Connectem els shaders amb el programa
	cout << "Fent link dels shaders..." << endl;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR: No s'ha pogut crear el link entre el programa y els shaders\n" << infoLog << endl;
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	return 0;
}

// Si es redimensiona la finestra, actualitzar el viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Funció per processar tots els inputs
void Joc::processInput() {
	// Si es pressiona ESC, es tanca la finestra
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Amb F1 es canvia entre mode normal i wireframe
	else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		if (!mode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		mode = !mode;
	}
}

int Joc::crearFinestra() {

	// Iniciem OpenGL
	glfwInit();

	// Expressem la versió de GLFW que volem, 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creem una nova finestra amb unes dimensions i un nom i que comenci en Windowed mode
	window = glfwCreateWindow(width, height, "MinecraftGL", NULL, NULL);

	// Si no aconseguim crear-la, terminem el programa
	if (window == NULL)
	{
		cout << "No s'ha pogut crear la finestra" << endl;
		glfwTerminate(); // Termina el context GLFW
		return -1;
	}
	// Fem que la finestra creada es torni el context actual
	glfwMakeContextCurrent(window);

	// Iniciem GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "No s'ha pogut iniciar GLAD" << endl;
		return -1;
	}

	// Li diem la mida al viewport, i des d'on comença
	glViewport(0, 0, width, height);

	// Assignem una funció per quan es redimensioni la finestra creada
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return 1;
}

void Joc::loop() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Chunk c;

	int fps = 0;
	auto start = chrono::system_clock::now();
	// El loop del joc, mentre no es tanqui la finestra...
	while (!glfwWindowShouldClose(window))
	{
		processInput();

		// Canvia el color del background
		glClearColor(0.4f, 0.2f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		c.render();

		glfwSwapBuffers(window); // Volcar l'array de color a la finestra
		glfwPollEvents(); // Processar events

		// Mostrem els frames que hi ha hagut en un segon (fps)
		chrono::duration<double> diff = chrono::system_clock::now() - start;
		if (diff.count() >= 1) { // Si la diferència és 1 és que ha passat un segon
			start = chrono::system_clock::now();
			cout << "Fps: " << fps << endl; // Mostrem els frames que hem pogut processar
			fps = 0; // Resetejem el comptador
		}
		fps++;
	}
}

void Joc::gameLoop() {
	cout << "Joc start" << endl;

	if (carregaShaders() == 0) {

		// El loop de veritat
		loop();

	}

	// Terminar el programa
	glfwTerminate();
}

