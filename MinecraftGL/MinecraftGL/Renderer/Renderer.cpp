#include "Renderer.h"

Renderer::~Renderer()
{
	
}

int Renderer::crearFinestra()
{
	// Iniciem GLFW
	glfwInit();

	// Expressem la versió de GLFW que volem, 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creem una nova finestra amb unes dimensions i un nom i que comenci en Windowed mode
	window = glfwCreateWindow(WIDTH, HEIGHT, "MinecraftGL", NULL, NULL); //glfwGetPrimaryMonitor()
	Recursos::width = WIDTH;
	Recursos::height = HEIGHT;

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
		glfwTerminate(); // Termina el context GLFW
		return -1;
	}

	// Li diem la mida al viewport, i des d'on comença
	glViewport(0, 0, WIDTH, HEIGHT);

	framebuffer = Framebuffer(WIDTH, HEIGHT);

	centrarFinestra();

	// Podem posar una icona a la finestra
	int iconwidth, iconheight, nrChannels;
	string path = "Textures/icon.png";
	unsigned char* img = stbi_load(path.c_str(), &iconwidth, &iconheight, &nrChannels, 4);
	GLFWimage icon;
	icon.width = iconwidth;
	icon.height = iconheight;
	icon.pixels = img;
	glfwSetWindowIcon(window, 1, &icon);

	return 1;
}

void Renderer::centrarFinestra()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glfwSetWindowPos(window, (mode->width-w)/2, (mode->height - h) / 2);
	cout << "Width: " << w << ", height: " << h << endl;

}


GLFWwindow* Renderer::finestra()
{
	return window;
}


int Renderer::carregaShaders()
{
	shaders[0] = ShaderProgram("VertexShader.vert", "FragmentShader.frag");
	shaders[1] = ShaderProgram("VertexPla.vert", "FragmentPla.frag");

	int totbe = 1, i=0;
	while (totbe && i<2) {
		totbe = shaders[i].carregaShaders();
		i++;
	}

	if (totbe) {
		usarShader();
	}
	
	texturaMon = Recursos::obtTextura("minecraft_transp.png");
	
	return totbe;
}

void Renderer::usarShader(int i)
{
	shaderActual = i;
	shaders[i].usar();
}

ShaderProgram* Renderer::obtShader()
{
	return &(shaders[shaderActual]);
}

unsigned int Renderer::obtenirUniform(const char* uniform) const
{
	return shaders[shaderActual].obtenirUniform(uniform);
}

void Renderer::colocarMat4(const string uniform, const glm::mat4 matriu)
{
	shaders[shaderActual].colocarMat4(uniform, matriu);
}

void Renderer::colocarInt(const string uniform, int n)
{
	shaders[shaderActual].colocarInt(uniform, n);
}

void Renderer::activaAigua(bool aigua)
{
	colocarInt("sotaAigua", aigua);
}

void Renderer::activaBounding(bool bounding)
{
	colocarInt("bounding", bounding);
}

void Renderer::activaNit(bool nit)
{
	colocarInt("nit",nit);
}

void Renderer::canviarColorLlum(const glm::vec3 color)
{
	glUniform3f(obtenirUniform("lightColor"), color[0], color[1], color[2]);
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
	glfwGetFramebufferSize(window, &w, &h);
	return (float)w/(float)h;
}

void Renderer::usarTexturaMon()
{
	texturaMon->use();
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