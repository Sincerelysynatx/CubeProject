#include "Cube.h"
#define DEG2RAD(x) (x * (D3DX_PI / 180.0f)) // Converts degrees to radians
#define RAD2DEG(x) (x * (180.0f / D3DX_PI)) // Converts radians to degrees
#define XRGB(r, g, b) D3DCOLOR_XRGB(r, g, b)

Cube::Cube()
{

}

Cube::Cube(float length, float height, float width, int x, int y, int z, float rotationX, float rotationY, bool render)
{

}

Cube::Cube(int x, int y, int z, bool render)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_render = render;
	position.x = x;
	position.y = y;
	position.z = z;
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
			*D3DXMatrixTranslation(&tM, m_x * 2.0f + position.x, m_y * 2.0f + position.y, m_z * 2.0f + position.z);
		d3dManager->getDevice().SetTransform(D3DTS_WORLD, &wM);
		for (int i = 0; i < 6; i++)
			d3dManager->getDevice().DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, i * 4, 2);
	}
}

void Cube::update(const float dt)
{
	//m_rotationX += DEG2RAD(0.2f * dt);
	//m_rotationY += DEG2RAD(0.5f * dt);
}

void Cube::move(const float dt, const float &x, const float &y, const float &z)
{
	position.x += x * dt;
	position.y += y * dt;
	position.z += z * dt;
}