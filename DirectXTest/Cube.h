#ifndef CUBE_H
#define CUBE_H

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#include "D3DManager.h"

class Cube
{
public:
	Cube();
	Cube(float length, float height, float width, int x, int y, int z, float rotationX, float rotationY, bool render);
	Cube(int x, int y, int z, bool render);
	~Cube();
	void render(D3DManager *d3dmanager, D3DXMATRIXA16 &bM, D3DXMATRIXA16 &wM, D3DXMATRIXA16 &rM1, D3DXMATRIXA16 &rM2, D3DXMATRIXA16 &tM);
	void update();

private:
	float m_length;
	float m_height;
	float m_width;
	int m_x, m_y, m_z;
	float m_rotationX, m_rotationY;
	bool m_render;
};

#endif