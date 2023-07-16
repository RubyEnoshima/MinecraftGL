#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(string vertex, string frag)
{
	vertexShaderSource = "Shaders/"+vertex;
	fragmentShaderSource = "Shaders/" + frag;
}

int ShaderProgram::carregaShaders() {
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
	cout << "Compilant Vertex Shader... ";
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

	cout << "Vertex Shader compilat" << endl;

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
	cout << "Compilant Fragment Shader... ";
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

	cout << "Fragment Shader compilat" << endl;

	// Connectem els shaders amb el programa
	cout << "Fent link dels shaders..." << endl;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR: No s'ha pogut crear el link entre el programa y els shaders\n" << infoLog << endl;
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	cout << "Shaders compilats: {" << vertexShaderSource << ", " << fragmentShaderSource << "}" << endl << endl << endl;

	return 1;
}

void ShaderProgram::usar()
{
	glUseProgram(shaderProgram);
}

int ShaderProgram::getProgram() const
{
	return shaderProgram;
}

unsigned int ShaderProgram::obtenirUniform(const char* uniform) const
{
	return glGetUniformLocation(shaderProgram, uniform);
}

void ShaderProgram::colocarMat4(const string uniform, const glm::mat4 matriu)
{
	glUniformMatrix4fv(obtenirUniform(uniform.c_str()), 1, GL_FALSE, &matriu[0][0]);
}

void ShaderProgram::colocarVec4(const string uniform, const glm::vec4 vector)
{
	glUniform4fv(obtenirUniform(uniform.c_str()), 1, &vector[0]);
}

void ShaderProgram::colocarVec3(const string uniform, const glm::vec3 vector)
{
	glUniform3fv(obtenirUniform(uniform.c_str()),1,&vector[0]);
}

void ShaderProgram::colocarVec2(const string uniform, const glm::vec2 vector)
{
	glUniform2fv(obtenirUniform(uniform.c_str()), 1, &vector[0]);
}

void ShaderProgram::colocarInt(const string uniform, int i)
{
	glUniform1i(obtenirUniform(uniform.c_str()),i);
}

