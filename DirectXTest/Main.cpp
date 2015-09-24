#include "Window.h"
#include "D3DManager.h"
#include "CustomVertexFormats.h"
#include "KeyCodes.h"
#include "Cube.h"
#include "Timer.h"
#include "Camera.h"
#include "Vector.h"
#include <vector>
#include <iostream>

#define DEG2RAD(x) (x * (D3DX_PI / 180.0f)) // Converts degrees to radians
#define RAD2DEG(x) (x * (180.0f / D3DX_PI)) // Converts radians to degrees

Window* window;
D3DManager* d3dManager;
IDirect3DVertexBuffer9* VBO = NULL;
IDirect3DIndexBuffer9* IBO = NULL;
Timer *timer;
std::vector<Cube*> cubes;
//CPos gCubePos(0, 0, 0); // Position of cube in the world
D3DXVECTOR3 viewVectors[3] = {
	D3DXVECTOR3(0.0f, 0.0f, -3.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 1.0f, 0.0f)
};
float deltaTime = 0;
float moveAmt = 10.0f * (1.0f / 60.0f);


void cleanup();
void quitWithError(LPCTSTR error);
void programLoop();
void prepareForDrawing();
void render();
void update();
void CameraMouseInput();
void initializeResources();
void initializeMatrices();
void releaseResources();
void initialize();
void getSupportedVindowResolutions();

LRESULT CALLBACK messageHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

int main()
{
	try {
		window = new Window(messageHandler, TEXT("DirectX Window"), 300, 300, 600, 600);
		d3dManager = new D3DManager(window->getHandle(), 600, 600, false);
		d3dManager->onDeviceLost = releaseResources;
		d3dManager->onDeviceReset = initialize;
		initialize();
	}
	catch (LPCTSTR error)
	{
		quitWithError(error);
	}
	getSupportedVindowResolutions();
	window->show();
	programLoop();
	cleanup();
	return EXIT_SUCCESS;
}

void programLoop() 
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg)); // Just incase there's something there.
	// Basically, we loop as long as we don't get the QUIT message.
	while (msg.message != WM_QUIT) {
		// Are there any messages waiting to be processed?
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// Translate it and send it off for processing.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			update();
			prepareForDrawing();
		}
	}
}

void update()
{
	//update timer
	timer->update();
	//get mouse input for the camera
	//CameraMouseInput();
	//d3dManager->setViewMatrix(gCamera);
	deltaTime = timer->getDeltaTime();
	for (auto cube : cubes)
	{
		cube->update(deltaTime);
	}
}

void prepareForDrawing()
{
	d3dManager->getDevice().Clear(
		// DWORD Count - Number of rectangles to clear.
		0,
		// const D3DRECT* pRects - The rectangles to clear.
		NULL, 
		// DWORD Flags - Special flags to use when clearing.
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		// D3DCOLOR Color - The colour to clear the target back buffer to.
		0x00000000, 
		// float Z - What value to clear the zbuffer to.
		1.0f, 
		// DWORD Stencil - What value to clear the stencil buffer to.
		0);
	d3dManager->getDevice().BeginScene();
	render();
	d3dManager->getDevice().EndScene();
	d3dManager->getDevice().Present(NULL, NULL, NULL, NULL);
}

void render()
{
	static D3DXMATRIXA16 baseMatrix, worldMatrix, rotationMatrix1, rotationMatrix2, translateMatrix;
	d3dManager->getDevice().GetTransform(D3DTS_WORLD, &baseMatrix);
	for (auto cube : cubes)
	{
		cube->render(d3dManager, baseMatrix, worldMatrix, rotationMatrix1, rotationMatrix2, translateMatrix);
	}
	d3dManager->getDevice().SetTransform(D3DTS_WORLD, &baseMatrix);

}

void CameraMouseInput()
{
	const float kMaxAngle = 89.0f;
	static float pitchAmt = 0.0f; // Amout we've looked up or down

								  // Get the middle of the screen
	int midScrX = GetSystemMetrics(SM_CXSCREEN) >> 1;
	int midScrY = GetSystemMetrics(SM_CYSCREEN) >> 1;

	float amt;
	POINT pt = { 0 };

	GetCursorPos(&pt); // Get the current mouse position

					   // Rotate left/right
	amt = float(pt.x - midScrX) * moveAmt;
	//gCamera->rotateY(DEG2RAD(amt), gCubePos);

	// Rotate up/down
	amt = float(midScrY - pt.y) * moveAmt;

	// Cap pitch
	if (pitchAmt + amt < -kMaxAngle)
	{
		amt = -kMaxAngle - pitchAmt;
		pitchAmt = -kMaxAngle;
	}
	else if (pitchAmt + amt > kMaxAngle)
	{
		amt = kMaxAngle - pitchAmt;
		pitchAmt = kMaxAngle;
	}
	else
	{
		pitchAmt += amt;
	}

	// Pitch the camera up/down
	//gCamera->pitch(DEG2RAD(amt), gCubePos);

	SetCursorPos(midScrX, midScrY); // Set our cursor back to the middle of the screen
}

LRESULT CALLBACK messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg) 
	{
		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			return 0;
		
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_F11:
					try {
						if (d3dManager->isFullScreen())
						{
							d3dManager->goWindowed(600, 600);
							window->setSize(600, 600);
						}
						else
						{
							d3dManager->goFullScreen(1920, 1080);
						}
					}
					catch (LPCTSTR error)
					{
						quitWithError(error);
					}
					return 0;
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					return 0;

					// C toggles the culling mode.
				case VK_C:
					d3dManager->toggleCullMode();
					return 0;

					// *NEW* - V changes the fill mode (Points, Wireframe, or Solid)
				case VK_V:
					d3dManager->toggleFillMode();
					return 0;

				case 'W':
				case VK_UP: // If they push up, move forward (Camera's +Z)
					viewVectors[0][2] += 0.5f;
					d3dManager->setViewMatrix(viewVectors);
					return 0;

				case 'S':
				case VK_DOWN: // If they push down, move backward (Camera's -Z)
					viewVectors[0][2] -= 0.5f;
					d3dManager->setViewMatrix(viewVectors);
					return 0;

				case 'D':
				case VK_RIGHT: // If they push right, move right (Camera's +X)
					viewVectors[0][0] += 0.5f;
					viewVectors[1][0] += 0.5f;
					d3dManager->setViewMatrix(viewVectors);
					return 0;

				case 'A':
				case VK_LEFT: // If they push left, move left (Camera's -X)
					viewVectors[0][0] -= 0.5f;
					viewVectors[1][0] -= 0.5f;
					d3dManager->setViewMatrix(viewVectors);
					return 0;
			}
	}
	// If we don't catch it, let the default message handler get it. That's this function.
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void initializeResources()
{
	Vertex_UD vertices[] = {
		{ -1.0f, 1.0f, -1.0f, 0xff00ff00 },
		{ 1.0f, 1.0f, -1.0f, 0xff0000ff },
		{ -1.0f, -1.0f, -1.0f, 0x00ffff00 },
		{ 1.0f, -1.0f, -1.0f, 0x0000ffff },

		{ -1.0f, 1.0f, 1.0f, 0xff00ff00 },
		{ 1.0f, 1.0f, 1.0f, 0xff0000ff },
		{ -1.0f, -1.0f, 1.0f, 0x00ffff00 },
		{ 1.0f, -1.0f, 1.0f, 0x0000ffff }
	};

	// With our vertices created, we can make our triangle buffer now.
	HRESULT result = d3dManager->getDevice().CreateVertexBuffer(
		// UINT length - The size of our new vertex buffer, in bytes.
		sizeof(vertices),
		// DWORD Usage - Some usage flags to set up.
		D3DUSAGE_WRITEONLY,
		// DWORD FVF - The Vertex Format to use for this buffer.
		vertices[0].FORMAT,
		// D3DPOOL Pool - What memory resource to use for this buffer.
		D3DPOOL_DEFAULT,
		// IDirect3DVertexBuffer9** ppVertexBuffer - The vertex buffer to create.
		&VBO,
		// HANDLE* pSharedHandle - Reserved. Must be NULL.
		NULL);

	// Any problems? We can use the D3DManager errorCheck to help.
	d3dManager->errorCheck(result, TEXT("initializeResources - Failed to CreateVertexBuffer!"));

	void* bufferMemory;

	result = VBO->Lock(
		// UINT OffsetToLock - At what index, in bytes, to place the lock.
		0, 
		// UINT SizeToLock - How many bytes of memory to lock.
		sizeof(vertices), 
		// void** ppbData - The resulting pointer to write.
		&bufferMemory, 
		// DWORD flags - Some special flags to use in obtaining the lock.
		NULL);

	d3dManager->errorCheck(result, TEXT("initializeResources - Failed to get a vertexBuffer lock!"));

	// Now that we have that the void pointer, we can copy out vertices right into it.
	memcpy(bufferMemory, vertices, sizeof(vertices));

	// Finally, we unlock the buffer. It is now ready for use.
	VBO->Unlock();

	d3dManager->getDevice().SetFVF(vertices[0].FORMAT);

	d3dManager->getDevice().SetStreamSource(
		// UINT StreamNumber - Which stream to set this to.
		0,
		// IDirect3DVertexBuffer9* pStreamData - The vertex buffer to use.
		VBO,
		// UINT OffsetInBytes - The offset to start reading from, in bytes.
		0,
		// UINT Stride - The size of a single vertex.
		vertices[0].STRIDE_SIZE);

	short indices[] = {
		0, 1, 2, 3, // Front
		1, 5, 3, 7, // Right
		4, 5, 0, 1, // Top
		5, 4, 7, 6, // Back
		4, 0, 6, 2, // Left
		2, 3, 6, 7 // Bottom
	};

	// Creating the index buffer is a lot like creating the vertex buffer.
	result = d3dManager->getDevice().CreateIndexBuffer(sizeof(indices), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &IBO, NULL);
	d3dManager->errorCheck(result, TEXT("initializeResources - Failed to CreateIndexBuffer!"));

	result = IBO->Lock(0, sizeof(indices), &bufferMemory, NULL);
	d3dManager->errorCheck(result, TEXT("initializeResources - Failed to get an indexBuffer lock!"));
	memcpy(bufferMemory, indices, sizeof(indices));
	IBO->Unlock();

	d3dManager->getDevice().SetFVF(vertices[0].FORMAT);
	d3dManager->getDevice().SetStreamSource(0, VBO, 0, vertices[0].STRIDE_SIZE);
	d3dManager->getDevice().SetIndices(IBO);
}

void initialize()
{
	initializeMatrices();
	initializeResources();
	float pointSize = 0.0f;
	timer = new Timer;
	for (int z = 0; z < 5; z++)
	{
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				cubes.push_back(new Cube(x, y, z, true));
			}
		}
	}
	d3dManager->getDevice().SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
}

// -------------------------------------------------
/* initializeMatrices */
// Initialize the view & projection matrices.
// -------------------------------------------------
void initializeMatrices() {
	/*
	D3DXVECTOR3 viewVectors[3] = {
		D3DXVECTOR3(0.0f, 0.0f, -3.5f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f)
	};
	*/

	d3dManager->setViewMatrix(viewVectors);
	d3dManager->setProjectionMatrix();
	d3dManager->getDevice().SetRenderState(D3DRS_LIGHTING, false);
}

void getSupportedVindowResolutions()
{
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	DWORD tempWidth = 0, tempHeight = 0;
	
	for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++) {
		if (dm.dmPelsWidth == tempWidth && dm.dmPelsHeight == tempHeight)
		{
			continue;
		}
		else
		{
			std::cout << "Mode #" << iModeNum << " = " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << std::endl;
			tempWidth = dm.dmPelsWidth; tempHeight = dm.dmPelsHeight;
		}
	}
}

void quitWithError(LPCTSTR error)
{
	// We'll use MessageBox to show the error to the screen.
	HWND parentWindow = NULL;
	if (window != NULL) parentWindow = window->getHandle();
	MessageBox(parentWindow, error, TEXT("Runtime Error!"), MB_OK | MB_ICONERROR);
	// Even though we're quitting on a failure, that's no reason to avoid cleaning up after
	// ourselves.
	cleanup();

	// Finally, force an exit with the Failure exit code.
	exit(EXIT_FAILURE);
}

void cleanup()
{
	if (window != NULL) { delete window; window = NULL; }
	if (d3dManager != NULL) { delete d3dManager; d3dManager = NULL; }
	delete timer;
	releaseResources();
}

void releaseResources()
{
	if (VBO != NULL) { VBO->Release(); VBO = NULL; }
	if (IBO != NULL) { IBO->Release(); IBO = NULL; }
}