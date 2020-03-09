#ifndef _INTERFACE_RENDERER_H_
#define _INTERFACE_RENDERER_H_

#ifdef _WIN32 
#define D3DDevice		ID3D11Device
#define D3DContext		ID3D11DeviceContext
#define D3DBuffer		ID3D11Buffer
#define VertexBuffer	ID3D11Buffer
#define IndexBuffer		ID3D11Buffer
#define D3DUsage		D3D11_USAGE
#define D3DBindFlag		D3D11_BIND_FLAG
#else
//#define D3DDevice
//#define D3DContext
//#define D3DBuffer
//#define VertexBuffer
//#define IndexBuffer
#endif

#include "../Scene.h"

class IRenderer {
public:
	virtual bool		Initialise(Scene& scene) = 0;
	virtual void		Update(float dt) = 0;
	virtual void		Render() = 0;
	virtual bool		CreateDevice() = 0;
	virtual bool		CreateResources() = 0;
	virtual bool		OnDeviceLost() = 0;
	virtual D3DDevice*	GetDevice() = 0;
	virtual D3DContext*	GetDeviceContext() = 0;
};

#endif //_INTERFACE_RENDERER_H_
