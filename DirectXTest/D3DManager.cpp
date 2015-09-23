#include "D3DManager.h"

D3DManager::D3DManager(HWND window, int width, int height, bool fullscreen)
{
	// So let's start by initializing the direct3D context.
	context = Direct3DCreate9(D3D_SDK_VERSION);

	// Was it successful?
	if (context == NULL) throw(TEXT("D3DManager Constructor: Failed to create a Direct3D Context!"));

	// UINT BackBufferWidth & BackBufferHeight - The size of the drawing area.
	parameters.BackBufferWidth = width;
	parameters.BackBufferHeight = height;
	// D3DFORMAT BackBufferFormat - The colour format of the back buffer.
	parameters.BackBufferFormat = D3DFMT_X8R8G8B8;

	// UINT BackBufferCount - The number of back buffers to use.
	parameters.BackBufferCount = 1;

	// D3DMULTISAMPLE_TYPE MultiSampleType - Sets the degree of anti-aliasing.
	parameters.MultiSampleType = D3DMULTISAMPLE_NONE;

	// DWORD MultiSampleQuality - The quality of anti-aliasing.
	parameters.MultiSampleQuality = 0;

	// D3DSWAPEFFECT SwapEffect - How D3D will deal with buffer swapping.
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// HWND hDeviceWindow - What window to tie this device to
	parameters.hDeviceWindow = window;

	// BOOL Windowed - Whether to run in Windowed, or Full Screen mode.
	parameters.Windowed = !fullscreen; // *NEW* - Specify as you will now.

	parameters.EnableAutoDepthStencil = true;

	parameters.AutoDepthStencilFormat = D3DFMT_D16;

	// DWORD Flags - Extra settings and flags to control.
	parameters.Flags = NULL;

	// UINT FullScreen_RefreshRateInHz - The refresh rate to use when in full screen.
	parameters.FullScreen_RefreshRateInHz = 0;

	// UINT PresentationInterval - How often to redraw the screen.
	parameters.PresentationInterval = (fullscreen)? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_ONE;
	//parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (fullscreen) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);

	HRESULT result = context->CreateDevice(
		// UINT adapter - Which adapter device to use.
		D3DADAPTER_DEFAULT,
		// D3DDEVTYPE DeviceType - What device to use to draw Direct3D.
		D3DDEVTYPE_HAL,
		// HWND hFocusWindow - The window to tie this device to.
		window,
		// DWORD BehaviourFlags - Extra flags and options you can use.
		D3DCREATE_MIXED_VERTEXPROCESSING,
		// D3DPRESENT_PARAMETERS* pPresentationParameters - Pointer to the Presentation Parameters to use.
		&parameters,
		// IDirect3DDevice9** ppReturnedDeviceInterface - Pointer to a Device pointer, used to return the new device.
		&device);
	// Now, our saved HRESULT will tell us if there are any problems.
	errorCheck(result, TEXT("D3DManager Constructor: Failed to create device!"));

	// If we're running in fullscreen, hide the mouse cursor. Otherwise, show it.
	if (fullscreen) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);
}

D3DManager::~D3DManager() {
	// We just release the device and context we've set up, and set the pointers to null for good measure.
	if (device != NULL) { device->Release(); device = NULL; }
	if (context != NULL) { context->Release(); context = NULL; }
}

// -------------------------------------------------
/* public errorCheck */
// Checks the supplied HRESULT for any Direct3D errors, and throws an exception if one is found.
// -------------------------------------------------
void D3DManager::errorCheck(HRESULT result, LPCTSTR debugInfo) {
	// D3D_OK is the only accepted HRESULT. Everything else is a failure.
	if (result == D3D_OK) return;
	while (ShowCursor(true) < 0);
	LPTSTR text;
	if (result == D3DERR_INVALIDCALL) text = TEXT("D3DERR_INVALIDCALL: Invalid Call or Parameter!");
	else if (result == D3DERR_NOTAVAILABLE) text = TEXT("D3DERR_NOTAVAILABLE: Parameter not supported!");
	else if (result == D3DERR_OUTOFVIDEOMEMORY) text = TEXT("D3DERR_OUTOFVIDEOMEMORY: Out of Video Memory!");
	else if (result == D3DERR_DEVICELOST) text = TEXT("D3DERR_DEVICELOST: The device is lost!");
	else if (result == D3DERR_DRIVERINTERNALERROR) text = TEXT("D3DERR_DRIVERINTERNALERROR: Internal Driver Error!");
	else if (result == D3DXERR_INVALIDDATA) text = TEXT("D3DXERR_INVALIDDATA: Invalid data passed to D3DX function!");
	else {
		text = new TCHAR[128];
		wsprintf(text, TEXT("Unknown Error: %X"), result);
	}

	LPTSTR error = new TCHAR[256];
	wsprintf(error, TEXT("%s\r\n%s"), debugInfo, text);
	throw(error);
}

// -------------------------------------------------
/* setViewMatrix */
// Sets the view matrix.
// -------------------------------------------------
void D3DManager::setViewMatrix(const D3DXVECTOR3& eye, const D3DXVECTOR3& lookAt, const D3DXVECTOR3& up) {
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &lookAt, &up);
	// Then it's the simple matter of making that the view matrix.
	device->SetTransform(D3DTS_VIEW, &viewMatrix);
}

void D3DManager::setViewMatrix(const Camera *camera) {
	D3DXMATRIXA16 viewMatrix;
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f); // World's up vector
	D3DXMatrixLookAtLH(&viewMatrix, (D3DXVECTOR3*)(&camera->getEye()), (D3DXVECTOR3*)(&camera->getTarget()), &up);
	// Then it's the simple matter of making that the view matrix.
	device->SetTransform(D3DTS_VIEW, &viewMatrix);
}

// -------------------------------------------------
/* setProjectionMatrix */
// Sets the projection matrix.
// -------------------------------------------------
void D3DManager::setProjectionMatrix(float fov, float aspectRatio, float zClose, float zFar) {
	D3DXMATRIXA16 projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, aspectRatio, zClose, zFar);

	// Then, set that to the projection matrix.
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

// -------------------------------------------------
/* public changeViewMode */
// Changes the view mode.
// -------------------------------------------------
void D3DManager::changeViewMode(int width, int height, bool fullscreen) {
	parameters.BackBufferWidth = width;
	parameters.BackBufferHeight = height;
	parameters.Windowed = !fullscreen;
	if (onDeviceLost) onDeviceLost();
	HRESULT result = device->Reset(&parameters);
	errorCheck(result, TEXT("D3DManager changeViewMode - Failed on device reset!"));

	// If we're running in fullscreen, hide the mouse cursor. Otherwise, show it.
	if (fullscreen) while (ShowCursor(false) >= 0);
	else while (ShowCursor(true) < 0);

	if (onDeviceReset) onDeviceReset();
}
// -------------------------------------------------
/* public toggleCullMode */
// This cycles through the culling modes, from CCW to CW to NONE.
// -------------------------------------------------
void D3DManager::toggleCullMode() {
	// First, get the current cull mode.
	DWORD cullMode;
	device->GetRenderState(D3DRS_CULLMODE, &cullMode);

	// Cycle...
	if (cullMode == D3DCULL_CCW) cullMode = D3DCULL_CW;
	else if (cullMode == D3DCULL_CW) cullMode = D3DCULL_NONE;
	else cullMode = D3DCULL_CCW;

	// And set.
	device->SetRenderState(D3DRS_CULLMODE, cullMode);
}

void D3DManager::toggleFillMode() {
	// First, get the current fill mode.
	DWORD fillMode; 
	device->GetRenderState(D3DRS_FILLMODE, &fillMode);

	// Cycle...
	if (fillMode == D3DFILL_SOLID) fillMode = D3DFILL_WIREFRAME;
	else if (fillMode == D3DFILL_WIREFRAME) fillMode = D3DFILL_POINT;
	else fillMode = D3DFILL_SOLID;

	// And set.
	device->SetRenderState(D3DRS_FILLMODE, fillMode);
}