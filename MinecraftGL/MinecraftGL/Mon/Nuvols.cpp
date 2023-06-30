#include "Nuvols.h"

Nuvols::Nuvols()
{
	offset = glm::vec2(0.0f);
	t = new Textura("nuvols.png");
	shader = new ShaderProgram("VertexNuvols.vert", "FragmentNuvols.frag");
    initRenderData();
}

Nuvols::~Nuvols()
{
    glDeleteVertexArrays(1, &VAOnuvols);
    glDeleteBuffers(1, &VBO);
	delete t;
	delete shader;
}

void Nuvols::render(const glm::mat4& view, const glm::mat4& projection)
{
	shader->usar();

	shader->colocarVec2("posicioSprite", offset);
	shader->colocarVec2("tamanyTextura", t->obtTamany());
	shader->colocarVec2("tamanySprite", glm::vec2(255));
    shader->colocarMat4("view", view);
    shader->colocarMat4("projection", projection);
    shader->colocarMat4("model",glm::mat4());

	t->use();

    glBindVertexArray(VAOnuvols);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Nuvols::update()
{
	offset.y += velocitat;
	offset.x += velocitat / 1.5;
}

void Nuvols::initRenderData()
{
    shader->carregaShaders();
    //shader->colocarInt("uTexture", 1);

    float vertices[] = {
        // pos      // tex
        0.0f, -100.0f, 0.0f, 1.0f,
        100.0f, -100.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f,
        100.0f, -100.0f, 1.0f, 1.0f,
        100.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &VAOnuvols);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAOnuvols);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}