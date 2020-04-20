#ifndef _D3D11_RENDERER_H_
#define _D3D11_RENDERER_H_

#include "SceneObjects/Camera.h"
#include "Interfaces/IRenderer.h"
#include "ConstBuffer.h"
#include <array>

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
	void									ClearFrame()override;
	void									SetBackColor(float r, float g, float b)override;

	bool									CreateDevice()override;
	bool									OnDeviceLost()override;

	IDevice*								GetDevice()override { return m_device.Get(); }
	IDevCon*								GetContext()override { return m_context.Get(); }

public:
	HRES									CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, IBuffer** pBuffer)override;
	HRES									CreateBlob(const char* path, IBlob** pBlob)override;

protected:
	std::array<float, 3>					m_backColour;
	CViewport								m_viewport;

	ComPtr<IDevice>							m_device;
	ComPtr<IDevCon>							m_context;
	ComPtr<ISwapChain>						m_swapChain;
	ComPtr<IRenderTargetView>				m_renderTargetView;
	ComPtr<IDepthStencilView>				m_depthStencilView;
	ComPtr<IRSState>						m_defaultRSState;

	ConstBuffer<CBPerFrame>					m_cbPerFrame;

	//std::vector<ComPtr<VertexBuffer>>		m_vertexBuffers;
	//std::vector<ComPtr<IndexBuffer>>		m_indexBuffers;
	//std::vector<ComPtr<VertexShader>>		m_vertexShaders;
	//std::vector<ComPtr<PixelShader>>		m_pixelShaders;
	//std::vector<ComPtr<BlendState>>			m_blendStates;
	//std::vector<ComPtr<RSState>>			m_rsStates;

};

#endif //_D3D11_RENDERER_H_
