#include "Renderer.h"

Renderer::~Renderer()
{
	
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

	// Assignem una funció per quan es redimensioni la finestra creada
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	framebuffer = Framebuffer(WIDTH, HEIGHT);

	centrarFinestra();

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
	//shaders[2] = ShaderProgram("VertexSprite.vert", "FragmentSprite.frag");

	int totbe = 1, i=0;
	while (totbe && i<MAX_SHADERS) {
		totbe = shaders[i].carregaShaders();
		i++;
	}

	if (totbe) {
		usarShader();
	}

	//Textura lightMap("lightMap.png");
	
	glActiveTexture(GL_TEXTURE0);
	texturaMon = Textura("minecraft_transp.png");
	texturaMon.use();
	glUniform1i(obtenirUniform("textura"), 0);

	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightMap.textura);*/

	//glUniform1i(obtenirUniform("lightMap"), 1);
	//cout << lightMap.textura << " " << textura.textura;

	
	return totbe;
}

void Renderer::usarShader(int i)
{
	shaderActual = i;
	shaders[i].usar();
}

unsigned int Renderer::obtenirUniform(const char* uniform) const
{
	return shaders[shaderActual].obtenirUniform(uniform);
}

void Renderer::colocarMat4(const string uniform, const glm::mat4 matriu)
{
	shaders[shaderActual].colocarMat4(uniform, matriu);
}

void Renderer::activaBounding(bool bounding)
{
	glUniform1i(obtenirUniform("bounding"), bounding);
}

void Renderer::canviarColorLlum(const glm::vec3 color)
{
	glUniform3f(obtenirUniform("lightColor"), color[0], color[1], color[2]);
}

void Renderer::canviarPosLlum(const glm::vec3 pos)
{
	glUniform3f(obtenirUniform("lightPos"), pos[0], pos[1], pos[2]);
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
	texturaMon.use();
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