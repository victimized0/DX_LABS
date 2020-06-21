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
	void									Present()final;

	void									ClearFrame()final;
	void									ClearGBuffer()final;

	void									UnbindRTVs()final;
	void									UnbindSRVs()final;
	void									UpdateSRVs()final;
	void									UpdateRTVs(IDepthStencilView* depthView)final;

	void									SetBackColor(float r, float g, float b)final;
	DirectX::SimpleMath::Vector2			GetScreenSize()final;

	bool									CreateDevice()final;
	bool									OnDeviceLost()final;

	IDevice*								GetDevice()final { return m_device.Get(); }
	IDevCon*								GetContext()final { return m_context.Get(); }
	IDXGISurface*							GetBackBuffer()final;
	ShadersManager*							GetShadersManager()final { return &m_shadersManager; }

public:
	HRES									CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, IBuffer** pBuffer)final;

protected:
	ComPtr<IRenderTargetView>				m_hdrRTV;
	ComPtr<IRenderTargetView>				m_quadHdrRTV;
	ComPtr<IRenderTargetView>				m_bloomRTV;

	ComPtr<IRenderTargetView>				m_sceneDiffuseAccRTV;
	ComPtr<IRenderTargetView>				m_sceneSpecularAccRTV;
	ComPtr<IRenderTargetView>				m_sceneNormalAccRTV;
	ComPtr<IRenderTargetView>				m_scenePositionAccRTV;

	ComPtr<IShaderResView>					m_hdrSRV;
	ComPtr<IShaderResView>					m_quadHdrSRV;
	ComPtr<IShaderResView>					m_bloomSRV;

	ComPtr<IShaderResView>					m_sceneDiffuseSRV;
	ComPtr<IShaderResView>					m_sceneSpecularSRV;
	ComPtr<IShaderResView>					m_sceneNormalSRV;
	ComPtr<IShaderResView>					m_scenePositionSRV;

	std::array<float, 4>					m_backColour;
	CViewport								m_viewport;
	CViewport								m_quadViewport;

	ComPtr<IDevice>							m_device;
	ComPtr<IDevCon>							m_context;
	ComPtr<ISwapChain>						m_swapChain;

	ComPtr<IRenderTargetView>				m_backBuffer;
	ComPtr<IDepthStencilView>				m_depthStencilView;

	ComPtr<IDepthStencilState>				m_readDSS;
	ComPtr<IDepthStencilState>				m_noDSState;
	ComPtr<IDepthStencilState>				m_defaultDSState;

	ComPtr<IBlendState>						m_oneAddBS;
	ComPtr<IRSState>						m_defaultRSState;
	ComPtr<ISamplerState>					m_defaultSampler;

	std::vector<IShaderResView*>			m_SRVs;
	std::vector<IRenderTargetView*>			m_RTVs;

	ShadersManager							m_shadersManager;
};

#endif //_D3D11_RENDERER_H_
