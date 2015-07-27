#include "Chunk.h"

Chunk::Chunk()
{
	m_cubes = new Cube**[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		m_cubes[i] = new Cube*[CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			m_cubes[i][j] = new Cube[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk()
{
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			delete[] m_cubes[i][j];
		}
		delete[] m_cubes[i];
	}
	delete[] m_cubes;
}