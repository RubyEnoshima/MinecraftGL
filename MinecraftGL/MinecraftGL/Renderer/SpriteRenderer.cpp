/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "SpriteRenderer.h"


SpriteRenderer::SpriteRenderer(ShaderProgram* _shader, Renderer* _renderer)
{
    shader = _shader;
    renderer = _renderer;
    width = (float)renderer->obtenirTamany().first;
    height = (float)renderer->obtenirTamany().second;
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &quadVAO);
    delete shader;
    for (auto sprite : Sprites)
    {
        delete sprite.second;
    }
}

void SpriteRenderer::render()
{
    for (auto sprite : Sprites)
    {
        DrawSprite(sprite.second);
    }
}

void SpriteRenderer::afegirSprite(Sprite* sprite)
{
    Sprites[sprite->nom] = sprite;
}

Sprite* SpriteRenderer::obtSprite(string nom) const
{
    map<string,Sprite*>::const_iterator it = Sprites.find(nom);
    if (it!=Sprites.end()) return it->second;
    return nullptr;
}

void SpriteRenderer::DrawSprite(Sprite* sprite)
{
    if (!sprite->visible) return;
    // prepare transformations
    shader->usar();
    glm::mat4 model = glm::mat4(1.0f);
    
    glm::vec2 tamanyEscalat = sprite->tamany * sprite->escala;
    glm::vec2 tamanyDividit = glm::vec2(tamanyEscalat.x/2, tamanyEscalat.y/2);

    //model = glm::translate(model, glm::vec3(normalitzarPos(glm::vec2(0), width, height), 0.0f));

    model = glm::translate(model, glm::vec3(normalitzarPos(sprite->pos, width, height), 0.0f));

    // Fem que l'sprite tingui l'escala exactament igual a la textura
    model = glm::scale(model, glm::vec3(tamanyEscalat.x / width, tamanyEscalat.y / height, 1.0f));
    // Centrem l'sprite si mode és true
    if(sprite->mode) model = glm::translate(model, glm::vec3(normalitzarPos(glm::vec2(0), width, height), 0.0f));

    //model = glm::translate(model, glm::vec3(normalitzarPos(tamanyDividit, sprite->tamany.x, sprite->tamany.y), 0.0f));


    // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 

    // Escalem segons la escala
    model = glm::scale(model, glm::vec3(sprite->escala,1));

    shader->colocarMat4("model", model);

    shader->colocarVec3("spriteColor", sprite->color);
    shader->colocarInt("image", 1);
    sprite->textura->use();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    shader->carregaShaders();
    shader->colocarInt("image",1);

    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, -1.0f, 0.0f, 1.0f, 
        1.0f, -1.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 0.0f, 0.0f, 0.0f, 
        1.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::vec2 SpriteRenderer::normalitzarPos(const glm::vec2& pos, int w, int h)
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

float SpriteRenderer::normalitzarPosUn(float pixel, float tamany)
{
    // Normalitzem de 0 a 1
    float normalized = pixel / tamany;

    return (normalized * 2.0f) - 1.0f;
}
