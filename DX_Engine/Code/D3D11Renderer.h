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

	void									SetBackColor(float r, float g, float b)final;
	void									SetSunLight(DirLight* pDirLight)final;

	bool									CreateDevice()final;
	bool									OnDeviceLost()final;

	IDevice*								GetDevice()final { return m_device.Get(); }
	IDevCon*								GetContext()final { return m_context.Get(); }

public:
	HRES									CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, IBuffer** pBuffer)final;
	HRES									CompileShader(const wchar_t* srcFile, const char* entryPoint, const char* profile, const std::vector<D3DShaderMacro>& macros, UINT flags, IBlob** ppBlob)final;
	HRES									CreateBlob(const char* path, IBlob** ppBlob)final;

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
	DirLight*								m_pDirLight;

};

#endif //_D3D11_RENDERER_H_
