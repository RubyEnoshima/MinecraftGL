#include "Renderer.h"

// Si es redimensiona la finestra, actualitzar el viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int Renderer::crearFinestra()
{
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

	centrarFinestra();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return 1;
}

void Renderer::centrarFinestra()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glfwSetWindowPos(window, (mode->width-w)/2, (mode->height - h) / 2);
}


GLFWwindow* Renderer::finestra()
{
	return window;
}

int Renderer::carregaShaders() {
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

	cout << "Fragment Shader compilat" << endl << endl;

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

	cout << "Shaders compilats." << endl << endl;

	return 1;
}

unsigned int Renderer::obtenirUniform(const char* uniform) const
{
	return glGetUniformLocation(shaderProgram,uniform);
}

void Renderer::colocarMat4(const string uniform, const glm::mat4 matriu)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, uniform.c_str()), 1, GL_FALSE, &matriu[0][0]);
}

void Renderer::canviarColor(const glm::vec4 color)
{
	glUniform4f(obtenirUniform("inColor"), color[0], color[1], color[2], color[3]);
}

float Renderer::rgb(int color) const
{
	return color/255.0f;
}

float Renderer::aspectRatio() const
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	return (float)w/(float)h;
}

pair<int, int> Renderer::obtenirTamany() const
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	return pair<int, int>(w, h);
}
