#ifndef _PONG_PCH_H_
#define _PONG_PCH_H_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NODRAWTEXT
//#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Direct2D
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

// Direct3D
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <wrl/client.h>
#include <assert.h>
#include <stdint.h>

#include <algorithm>
#include <memory>
#include <exception>
#include <vector>
#include <fstream>
#include <string>
#include <memory>

#include "VertexFormats.h"
#include "Environment.h"

inline void ThrowIfFailed(HRESULT hr)
{
#ifdef _DEBUG
	if (FAILED(hr))
	{
		// Set a breakpoint on this line to catch DirectX API errors
		throw std::exception();
	}
#else
	UNREFERENCED_PARAMETER(hr);
#endif
}

#endif //_PONG_PCH_H_
