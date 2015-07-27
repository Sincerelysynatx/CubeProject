#ifndef CHUNK_H
#define CHUNK_H
#include "Cube.h"

class Chunk
{
public:
	Chunk();
	~Chunk();
	void update(float dt);
	void render();
	static const int CHUNK_SIZE = 16;
private:
	Cube ***m_cubes;
};

#endif