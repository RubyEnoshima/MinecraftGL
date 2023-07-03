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
    glDeleteBuffers(1, &VBO);
    delete shader;
    for (pair<string,Sprite*> sprite : Sprites)
    {
        delete sprite.second;
    }
}

void SpriteRenderer::render()
{
    for (auto it = SpritesOrdenats.rbegin(); it != SpritesOrdenats.rend(); it++) {
        DrawSprite(it->second);
    }
}

void SpriteRenderer::afegirSprite(Sprite* sprite)
{
    Sprites[sprite->nom] = sprite;
    SpritesOrdenats.emplace(sprite->indexZ, sprite);
}

Sprite* SpriteRenderer::obtSprite(string nom) const
{
    map<string,Sprite*>::const_iterator it = Sprites.find(nom);
    if (it!=Sprites.end()) return it->second;
    return nullptr;
}

void SpriteRenderer::canviaIndex(string nom, int nouIndex)
{
    auto it = SpritesOrdenats.begin();
    while (it != SpritesOrdenats.end()) {
        if (it->second->nom == nom) {
            SpritesOrdenats.emplace_hint(it, nouIndex, it->second);
            SpritesOrdenats.erase(it);
            break;
        }
        it++;
    }
}

void SpriteRenderer::DrawSprite(Sprite* sprite)
{
    if (!sprite->visible) return;
    shader->usar();

    glm::mat4 model = glm::mat4(1.0f);

    // Movem l'sprite on toqui.
    model = glm::translate(model, glm::vec3(normalitzarPos(sprite->pos, width, height), 0.0f));

    // Fem que l'sprite tingui l'escala exactament igual a la textura. Si no fes això, l'sprite ocuparia un quart de la pantalla.
    model = glm::scale(model, glm::vec3(sprite->tamany.x / width, sprite->tamany.y / height, 1.0f));

    // Escalem l'sprite
    model = glm::scale(model, glm::vec3(sprite->escala, 1));

    // Centrem l'sprite si mode és true
    if (sprite->centrat) model = glm::translate(model, glm::vec3(normalitzarPos(glm::vec2(width/4,height/4), width, height), 0.0f));

    // Podriem rotar l'sprite, però de moment no ens interesa
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 

    shader->colocarMat4("model", model);

    shader->colocarVec4("spriteColor", sprite->color);
    
    sprite->textura->use();
    // Si la textura és un mapa de sprites, li podem dir quin volem
    {
        shader->colocarVec2("posicioSprite", sprite->posicioMapa);
        shader->colocarVec2("tamanySprite", sprite->tamanyMapa);
        shader->colocarVec2("tamanyTextura", sprite->textura->obtTamany());
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    shader->carregaShaders();
    shader->colocarInt("image",1);

    // Tots els sprites seran iguals: un quadrat que comença a la cantonada esq sup
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