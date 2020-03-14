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

	VertexShader*							GetVertexShader	(int vsId)	override	{ return m_vertexShaders[vsId].Get();	}
	PixelShader*							GetPixelShader	(int psId)	override	{ return m_pixelShaders	[psId].Get();	}
	VertexBuffer*							GetVertexBuffer	(int vbId)	override	{ return m_vertexBuffers[vbId].Get();	}
	IndexBuffer*							GetIndexBuffer	(int ibId)	override	{ return m_indexBuffers	[ibId].Get();	}
	InputLayout*							GetInputLayout	(int ilId)	override	{ return m_inputLayouts	[ilId].Get();	}
	RSState*								GetRSState		(int rssId)	override	{ return m_rsStates		[rssId].Get();	}

	int										CreateVertexBuffer(size_t size, size_t strideSize, const void* pData)override;
	int										CreateIndexBuffer(size_t size, const void* pData)override;
	int										CreateInputLayout(InputElementDesc* desc, size_t arrSize, D3DBlob* shaderBlob)override;
	int										CreateRSState(D3DRSDesc* desc)override;
	int										CreateVertexShader(const char* path, D3DBlob** ppBlob)override;
	int										CreatePixelShader(const char* path)override;

protected:
	HRESULT									CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, D3DBuffer** pBuffer);
	HRESULT									CreateBlob(const char* path, D3DBlob** pBlob);

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

	std::vector<ComPtr<VertexBuffer>>		m_vertexBuffers;
	std::vector<ComPtr<IndexBuffer>>		m_indexBuffers;
	std::vector<ComPtr<InputLayout>>		m_inputLayouts;
	std::vector<ComPtr<RSState>>			m_rsStates;
	std::vector<ComPtr<VertexShader>>		m_vertexShaders;
	std::vector<ComPtr<PixelShader>>		m_pixelShaders;

};

#endif //_RENDERER_H_
