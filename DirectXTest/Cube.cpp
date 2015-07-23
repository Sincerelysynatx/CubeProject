#include "Cube.h"

Cube::Cube(float length, float height, float width, int x, int y, int z, float rotationX, float rotationY, bool render)
{

}

Cube::Cube(int x, int y, int z, bool render)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_render = render;
	m_rotationX = 0;
	m_rotationY = 0;
}

Cube::~Cube()
{

}

void Cube::render(D3DManager *d3dManager, D3DXMATRIXA16 &bM, D3DXMATRIXA16 &wM, D3DXMATRIXA16 &rM1, D3DXMATRIXA16 &rM2, D3DXMATRIXA16 &tM)
{
	if (m_render)
	{
		wM = bM * *D3DXMatrixRotationX(&rM2, m_rotationX) *
			*D3DXMatrixRotationY(&rM1, m_rotationY) *
			*D3DXMatrixTranslation(&tM, m_x * 2, m_y * 2, m_z * 2);
		d3dManager->getDevice().SetTransform(D3DTS_WORLD, &wM);
		for (int i = 0; i < 6; i++)
			d3dManager->getDevice().DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, i * 4, 2);
	}
}