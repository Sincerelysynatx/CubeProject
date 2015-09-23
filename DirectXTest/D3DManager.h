#ifndef D3D_MANAGER_H
#define D3D_MANAGER_H

#include <d3d9.h>
#include <d3dx9.h> // *NEW* - For much easier control over Matrices.
#include "Camera.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib") // *NEW* - For much easier control over Matrices.

class D3DManager {

	// The one is for when a device is about to be reset (and hence will become lost).
	typedef void(*ON_DEVICE_LOST)();

	// The other is for after the device is actually (and successfully) reset.
	typedef void(*ON_DEVICE_RESET)();

public:
	//*** Constructor/Destructor ***
	D3DManager(HWND window, int width, int height, bool fullscreen);
	~D3DManager();

	//*** Accessors/Modifiers ***
	IDirect3DDevice9& getDevice() { return *device; }

	//*** Methods ***
	// This method returns if there's no errors, and throws an exception if there is.
	void errorCheck(HRESULT result, LPCTSTR debugInfo);

	void setViewMatrix(const D3DXVECTOR3& eye, const D3DXVECTOR3& lookAt, const D3DXVECTOR3& up);
	void setViewMatrix(D3DXVECTOR3 vector[3]) 
	{ 
		setViewMatrix(vector[0], vector[1], vector[2]); 
	}
	void setViewMatrix(const Camera *camera);
	void setProjectionMatrix(float fov, float aspectRatio, float zClose, float zFar);
	void setProjectionMatrix(float fov, float zClose, float zFar) 
	{
		setProjectionMatrix(fov, parameters.BackBufferWidth / (float)parameters.BackBufferHeight, zClose, zFar);
	}
	void setProjectionMatrix(float zClose, float zFar) {
		setProjectionMatrix(D3DX_PI / 4.0f, parameters.BackBufferWidth / (float)parameters.BackBufferHeight, zClose, zFar);
	}
	void setProjectionMatrix() {
		setProjectionMatrix(D3DX_PI / 4.0f, parameters.BackBufferWidth / (float)parameters.BackBufferHeight, 0.5f, 100.0f);
	}

	bool isFullScreen() { return !parameters.Windowed; }
	void changeViewMode(int width, int height, bool fullscreen);
	void goFullScreen(int width, int height) { changeViewMode(width, height, true); }
	void goWindowed(int width, int height) { changeViewMode(width, height, false); }
	
	void toggleCullMode();
	void toggleFillMode();

	ON_DEVICE_LOST onDeviceLost;
	ON_DEVICE_RESET onDeviceReset;

protected:
	IDirect3D9* context;
	IDirect3DDevice9* device;
	D3DPRESENT_PARAMETERS parameters;
};
#endif