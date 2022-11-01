#pragma once
#include "Chunk2.h"
#include "Renderer.h"

#define XC 16
#define YC 16

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

