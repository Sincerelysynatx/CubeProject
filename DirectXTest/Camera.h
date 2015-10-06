#ifndef CAMERA_H
#define CAMERA_H

#include <d3d9.h>
#include <d3dx9.h> // *NEW* - For much easier control over Matrices.
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib") // *NEW* - For much easier control over Matrices.

class Camera
{
public:
	Camera();
	Camera(float pitch, float yaw, D3DXVECTOR3 position);
	~Camera();
	void rotateYaw(float amt);
private:
	float m_yaw;
	float m_pitch;
	float m_roll;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 xAxis;
	D3DXVECTOR3 yAxis;
	D3DXVECTOR3 zAxis;
};

#endif