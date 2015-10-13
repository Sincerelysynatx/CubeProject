#ifndef CAMERA_H
#define CAMERA_H

#include <d3d9.h>
#include <d3dx9.h> // *NEW* - For much easier control over Matrices.
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib") // *NEW* - For much easier control over Matrices.

const float cameraDistance = 2.5f;
const D3DXVECTOR3 worldUp = { 0.0f, 1.0f, 0.0f };

class Camera
{
public:
	Camera();
	void rotateY(float angle, const D3DXVECTOR3 &target);
	void setPitch(float angle, const D3DXVECTOR3 &target);
	void setEye(const D3DXVECTOR3 &eyePos) { m_position = eyePos; }
	void setTarget(const D3DXVECTOR3 &target);
	void normalize(D3DXVECTOR3 &vec);
	D3DXVECTOR3 getTarget() const { return m_position + (m_forward * cameraDistance); }
	D3DXVECTOR3 getPos() const { return m_position; }
	D3DXVECTOR3 getCamForward() const { return m_forward; }
	D3DXVECTOR3 getCamUp() const { return m_up; }
	D3DXVECTOR3 getCamRight() const { return m_right; }

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_right;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_forward;

};

extern Camera *camera;

#endif