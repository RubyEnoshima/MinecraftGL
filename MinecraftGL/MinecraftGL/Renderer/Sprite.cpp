#include "Sprite.h"

void Sprite::teletransportar(const glm::vec2& novaPos)
{
    pos = novaPos;
    transformar();
}

void Sprite::moure(const glm::vec2& offset)
{
    pos += offset;
    transformar();
}

void Sprite::escalar(const glm::vec2& novaEscala)
{
    escala = novaEscala;
    transformar();
}

void Sprite::centrar(bool centrar)
{
    centrat = centrar;
    transformar();
}

glm::vec2 Sprite::obtPos() const
{
    return pos;
}

glm::vec2 Sprite::obtTamany() const
{
    return tamany;
}

void Sprite::transformar()
{
    model = glm::mat4();
    // Movem l'sprite on toqui.
    model = glm::translate(model, glm::vec3(normalitzarPos(pos, Recursos::width, Recursos::height), 0.0f));

    // Fem que l'sprite tingui l'escala exactament igual a la textura. Si no fes això, l'sprite ocuparia un quart de la pantalla.
    model = glm::scale(model, glm::vec3(tamany.x / Recursos::width, tamany.y / Recursos::height, 1.0f));

    // Escalem l'sprite
    model = glm::scale(model, glm::vec3(escala, 1));

    // Centrem l'sprite si mode és true
    if (centrat) model = glm::translate(model, glm::vec3(normalitzarPos(glm::vec2(Recursos::width / 4, Recursos::height / 4), Recursos::width, Recursos::height), 0.0f));
}

glm::vec2 Sprite::normalitzarPos(const glm::vec2& pos, int w, int h)
{
    // Normalitzem de 0 a 1
    float normalizedX = pos.x / w;
    float normalizedY = 1 - pos.y / h;

    // Normalitzem de -1 a 1
    glm::vec2 normalizedCoords;
    normalizedCoords.x = (normalizedX * 2.0f) - 1.0f;
    normalizedCoords.y = (normalizedY * 2.0f) - 1.0f;

    return normalizedCoords;
}
