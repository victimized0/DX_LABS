#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "SceneObjects/Camera.h"
#include "Interfaces/IRenderer.h"
#include "ConstBuffer.h"

#define RES_SUCCESS	0
#define RES_FAILED	-1

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class D3D11Renderer : public IRenderer {
public:
											D3D11Renderer();
	virtual									~D3D11Renderer();
											
											D3D11Renderer(D3D11Renderer const&)				= delete;
											D3D11Renderer& operator=(D3D11Renderer const&)	= delete;
											D3D11Renderer(D3D11Renderer&&)					= delete;
											D3D11Renderer& operator=(D3D11Renderer&&)		= delete;

public:
	bool									Initialise()override;
	void									Render()override;

	bool									CreateDevice()override;
	bool									CreateResources()override;
	bool									OnDeviceLost()override;

	D3DDevice*								GetDevice()override { return m_device.Get(); }
	D3DContext*								GetDeviceContext()override { return m_context.Get(); }

public:
	HRES									CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, D3DBuffer** pBuffer)override;
	HRES									CreateBlob(const char* path, D3DBlob** pBlob)override;

protected:
	UINT									m_buffersCount;
	UINT									m_msaa4xQuality;
	bool									m_enable4xMSAA;

	D3DViewport								m_viewport;
	D3DDriverType							m_driverType;
	D3DFeatureLevel							m_featureLevel;

	ComPtr<D3DDevice>						m_device;
	ComPtr<D3DContext>						m_context;
	ComPtr<D3DSwapChain>					m_swapChain;
	ComPtr<RenderTargetView>				m_renderTargetView;
	ComPtr<DepthStencilView>				m_depthStencilView;

};

#endif //_RENDERER_H_
