#ifndef CUSTOM_VERTEX_FORMATS_H
#define CUSTOM_VERTEX_FORMATS_H

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

struct Vertex_TD {
	float x, y, z, rhw;
	DWORD colour;
	static const DWORD FORMAT = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 20;
};

struct Vertex_UD {
	float x, y, z;
	DWORD colour;
	static const DWORD FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static const int STRIDE_SIZE = 16;
};
#endif