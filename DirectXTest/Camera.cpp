#include "Camera.h"

Camera::Camera(float pitch, float yaw, D3DXVECTOR3 position)
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_roll = 0x0f;
	m_position = position;
	xAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	yAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	zAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void Camera::rotateYaw(float amt)
{

}