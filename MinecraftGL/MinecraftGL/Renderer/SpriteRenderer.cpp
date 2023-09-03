#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
    shader = new ShaderProgram("VertexSprite.vert", "FragmentSprite.frag");
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
    eliminaSprite(sprite->nom);
    Sprites[sprite->nom] = sprite;
    SpritesOrdenats.emplace(sprite->indexZ, sprite);
}

void SpriteRenderer::eliminaSprite(string nom)
{
    if (Sprites.empty()) return;
    auto it = SpritesOrdenats.begin();
    while (it != SpritesOrdenats.end()) {
        if (it->second->nom == nom) {
            SpritesOrdenats.erase(it);
            break;
        }
        it++;
    }
    delete Sprites[nom];
    Sprites.erase(nom);

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

    // Podriem rotar l'sprite, però de moment no ens interesa
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 

    shader->colocarMat4("model", sprite->model);

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