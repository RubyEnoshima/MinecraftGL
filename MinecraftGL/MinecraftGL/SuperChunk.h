#pragma once
#include "Chunk2.h"
#include "Renderer.h"

#define XC 7
#define YC 7

class SuperChunk
{
public:
	SuperChunk(Renderer* _renderer);

	void update();
	void render();
private:
	Chunk2* Chunks[XC][YC];
	Renderer* renderer;
};

