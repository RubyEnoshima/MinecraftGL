#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

using namespace std;

// Si es redimensiona la finestra, actualitzar el viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Funció per processar tots els inputs
void processInput(GLFWwindow* window)
{
	// Si es pressiona ESC, es tanca la finestra
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
}


int main() {

	// Iniciem OpenGL
	glfwInit();

	// Expressem la versió de GLFW que volem, 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creem una nova finestra amb unes dimensions i un nom i que comenci en Windowed mode
	GLFWwindow* window = glfwCreateWindow(800, 600, "MinecraftGL", NULL, NULL);

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
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "No s'ha pogut iniciar GLAD" << endl;
		return -1;
	}
	
	// Li diem la mida al viewport, i des d'on comença
	glViewport(0,0,800,600);
	
	// Assignem una funció per quan es redimensioni la finestra creada
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	cout << "Joc start" << endl; 
	int fps = 0;
	auto start = chrono::system_clock::now();

	// El loop del joc, mentre no es tanqui la finestra...
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.4,0.2,0.7,1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window); // Volcar l'array de color a la finestra
		glfwPollEvents(); // Processar events
		
		// Mostrem els frames que hi ha hagut en un segon (fps)
		std::chrono::duration<double> diff = chrono::system_clock::now() - start;
		if (diff.count() >= 1) { // Si la diferència és 1 és que ha passat un segon
			start = chrono::system_clock::now();
			cout << fps << endl; // Mostrem els frames que hem pogut processar
			fps = 0; // Resetejem el comptador
		}
		fps++;
	}

	// Terminar el programa
	glfwTerminate();

	return 0;
}