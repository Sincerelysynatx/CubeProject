#include "Camera.h"
#include <iostream>

inline bool ToleranceEqual(float a, float b, float t)
{
	return ((a > b - t) && (a < b + t));
}

Camera::Camera()
{
	m_right = { 1.0f, 0.0f, 0.0f };
	m_up = { 0.0f, 1.0f, 0.0f };
	m_forward = { 0.0f, 0.0f, 1.0f };
	m_position = { 0.0f, 0.0f, -cameraDistance };
}

void Camera::rotateY(float angle, const D3DXVECTOR3 &target)
{
	float xxx, zzz;
	float sinAng, cosAng;

	//std::cout << target.x << ", " << target.y << ", " << target.z << std::endl;

	sinAng = sinf(angle);
	cosAng = cosf(angle);

	//std::cout << sinAng << "," << cosAng << std::endl;

	m_position -= target;

	xxx = m_position.x;
	zzz = m_position.z;

	m_position.x = xxx * cosAng + zzz * sinAng;
	m_position.z = xxx * -sinAng + zzz * cosAng;

	m_position += target;

	setTarget(target);
}

void Camera::setPitch(float angle, const D3DXVECTOR3 &target)
{
	float sinAng = sinf(angle);
	float cosAng = cosf(angle);

	m_position -= target;

	float xxx = (m_right.x * m_right.x * (1.0f - cosAng) + cosAng) * m_position.x +
				(m_right.x * m_right.y * (1.0f - cosAng) + m_right.z * sinAng) * m_position.y +
				(m_right.x * m_right.z * (1.0f - cosAng) - m_right.y * sinAng) * m_position.z;

	float yyy = (m_right.x * m_right.y * (1.0f - cosAng) - m_right.z * sinAng) * m_position.x +
				(m_right.y * m_right.y * (1.0f - cosAng) + cosAng) * m_position.y +
				(m_right.y * m_right.z * (1.0f - cosAng) + m_right.x * sinAng) * m_position.z;

	float zzz = (m_right.x * m_right.z * (1.0f - cosAng) + m_right.y * sinAng) * m_position.x +
				(m_right.y * m_right.z * (1.0f - cosAng) - m_right.x * sinAng) * m_position.y +
				(m_right.z * m_right.z * (1.0f - cosAng) + cosAng) * m_position.z;

	m_position = { xxx, yyy, zzz };
	m_position += target;

	D3DXVECTOR3 vec = m_position - target;

	normalize(vec);

	m_position = target + (vec * cameraDistance);

	setTarget(target);
}

void Camera::setTarget(const D3DXVECTOR3 &target)
{
	m_forward = target - m_position;
	normalize(m_forward);

	D3DXVec3Cross(&m_right, &m_right, &m_forward);
	normalize(m_right);

	D3DXVec3Cross(&m_up, &m_up, &m_right);
}

void Camera::normalize(D3DXVECTOR3 &vec)
{
	float magnitude = sqrtf((m_position.x * m_position.x) + (m_position.y * m_position.y) + (m_position.y * m_position.y));

	if (ToleranceEqual(magnitude, 0.0f, .001f))
		return; // can't normalize a vector of zero magnitude

	float oneOverLen = 1.0f / magnitude;

	vec.x *= oneOverLen;
	vec.y *= oneOverLen;
	vec.z *= oneOverLen;
}

Camera tempCam;
Camera *camera = &tempCam;