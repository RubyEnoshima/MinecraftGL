#include "Renderer.h"

Renderer::~Renderer()
{
	glDeleteBuffers(1,&VAO);
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
	window = glfwCreateWindow(WIDTH, HEIGHT, "MinecraftGL", NULL, NULL);

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
	glViewport(0, 0, WIDTH, HEIGHT);

	// Assignem una funció per quan es redimensioni la finestra creada
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	framebuffer = Framebuffer(WIDTH, HEIGHT);

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


int Renderer::carregaShaders()
{
	shaders[0] = ShaderProgram("VertexShader.vert", "FragmentShader.frag");
	shaders[1] = ShaderProgram("VertexPla.vert", "FragmentPla.frag");
	int totbe = shaders[0].carregaShaders();
	shaders[1].carregaShaders();

	if (totbe) {
		usarShader();
	}

	Textura t("minecraft_transp.png");

	return totbe;
}

void Renderer::usarShader(int i)
{
	shaderActual = shaders[i].getProgram();
	//cout << "Usant " << shaderActual << endl;
	shaders[i].usar();
}

unsigned int Renderer::obtenirUniform(const char* uniform) const
{
	return glGetUniformLocation(shaderActual,uniform);
}

void Renderer::colocarMat4(const string uniform, const glm::mat4 matriu)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderActual, uniform.c_str()), 1, GL_FALSE, &matriu[0][0]);
}

void Renderer::canviarColor(const glm::vec4 color)
{
	glUniform4f(obtenirUniform("inColor"), color[0], color[1], color[2], color[3]);
}

float Renderer::rgb(int color) const
{
	return color/255.0f;
}

void Renderer::DibuixarDarrera() const
{
	framebuffer.Unir();
}

void Renderer::DibuixarFront() const
{
	framebuffer.Desunir();
}

float Renderer::aspectRatio() const
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	cout << "Width: " << w << ", height: " << h << endl;
	return (float)w/(float)h;
}

pair<int, int> Renderer::obtenirTamany() const
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return pair<int, int>(w, h);
}

pair<int, int> Renderer::obtenirCentre() const
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	return pair<int, int>(w/2, h/2);
}