#include "Nuvols.h"

Nuvols::Nuvols(const glm::mat4& _projection)
{
	offset = glm::vec2((float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX));
	t = Recursos::obtTextura("nuvols.png");
	shader = new ShaderProgram("VertexNuvols.vert", "FragmentNuvols.frag");
    initRenderData();
    projection = _projection;

}

Nuvols::~Nuvols()
{
    glDeleteVertexArrays(1, &VAOnuvols);
    glDeleteBuffers(1, &VBO);
	delete t;
	delete shader;
}

void Nuvols::render(const glm::mat4& view)
{
	shader->usar();

	shader->colocarVec2("posicioSprite", offset);
	shader->colocarVec2("tamanyTextura", t->obtTamany());
    shader->colocarMat4("view", view);
    shader->colocarVec2("tamanySprite", glm::vec2(15));
    shader->colocarMat4("projection", projection);
    shader->colocarInt("nit", nit);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(pos.x,0,pos.y));
    shader->colocarMat4("model",model);

	t->use();

    glBindVertexArray(VAOnuvols);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Nuvols::update(const glm::vec2& _pos, float deltaTime)
{
	offset.y += (pos.y - _pos.y)/25 + velocitat * deltaTime;
	offset.x += (_pos.x - pos.x)/25 + velocitat * deltaTime / 3;
    pos = _pos;
}

void Nuvols::canviaHora(bool _nit)
{
    nit = _nit;
}

void Nuvols::initRenderData()
{
    shader->carregaShaders();

    float altura = 200;
    float vertices[] = {
        // pos      // tex
        -500, altura, -500.0f, 0.0f, 1.0f,
        500.0f, altura, -500.0f, 1.0f, 1.0f,
        -500, altura, 500, 0.0f, 0.0f,

        -500, altura, 500, 0.0f, 0.0f,
        500.0f, altura, -500.0f, 1.0f, 1.0f,
        500.0f, altura, 500, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &VAOnuvols);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAOnuvols);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}