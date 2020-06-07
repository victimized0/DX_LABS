#ifndef _D3D11_RENDERER_H_
#define _D3D11_RENDERER_H_

#include "SceneObjects/Camera.h"
#include "Interfaces/IRenderer.h"

#include <array>

#define RES_SUCCESS	0
#define RES_FAILED	-1

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class D3D11Renderer final : public IRenderer {
public:
											D3D11Renderer();
	virtual									~D3D11Renderer();
											
											D3D11Renderer(D3D11Renderer const&)				= delete;
											D3D11Renderer& operator=(D3D11Renderer const&)	= delete;
											D3D11Renderer(D3D11Renderer&&)					= delete;
											D3D11Renderer& operator=(D3D11Renderer&&)		= delete;

public:
	bool									Initialise()final;
	void									Render()final;
	void									ClearFrame()final;
	void									ClearGBuffer()final;

	void									SetBackColor(float r, float g, float b)final;

	bool									CreateDevice()final;
	bool									OnDeviceLost()final;

	IDevice*								GetDevice()final { return m_device.Get(); }
	IDevCon*								GetContext()final { return m_context.Get(); }
	ShadersManager*							GetShadersManager()final { return &m_shadersManager; }

public:
	HRES									CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, IBuffer** pBuffer)final;

protected:
	ComPtr<IRenderTargetView>				m_hdrRTV;
	ComPtr<IRenderTargetView>				m_diffuseAccRTV;
	ComPtr<IRenderTargetView>				m_specularAccRTV;
	ComPtr<IRenderTargetView>				m_normalAccRTV;
	ComPtr<IRenderTargetView>				m_positionAccRTV;

	ComPtr<IShaderResView>					m_hdrSRV;
	ComPtr<IShaderResView>					m_diffuseAccSRV;
	ComPtr<IShaderResView>					m_specularAccSRV;
	ComPtr<IShaderResView>					m_normalAccSRV;
	ComPtr<IShaderResView>					m_positionAccSRV;

	std::array<float, 3>					m_backColour;
	CViewport								m_viewport;
	ComPtr<IDevice>							m_device;
	ComPtr<IDevCon>							m_context;
	ComPtr<ISwapChain>						m_swapChain;
	ComPtr<IRenderTargetView>				m_renderTargetView;
	ComPtr<IDepthStencilView>				m_depthStencilView;
	ComPtr<IDepthStencilState>				m_noDSState;
	ComPtr<IDepthStencilState>				m_defaultDSState;
	ComPtr<IRSState>						m_defaultRSState;
	ComPtr<ISamplerState>					m_defaultSampler;

	ShadersManager							m_shadersManager;
};

#endif //_D3D11_RENDERER_H_
