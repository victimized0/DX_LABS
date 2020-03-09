#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Scene.h"
#include "SceneObjects/Camera.h"
#include "Interfaces/IRenderer.h"
#include "ConstBuffer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class D3D11Renderer : public IRenderer {
public:
									D3D11Renderer();
	virtual							~D3D11Renderer();
									
									D3D11Renderer(D3D11Renderer const&)				= delete;
									D3D11Renderer& operator=(D3D11Renderer const&)	= delete;
									D3D11Renderer(D3D11Renderer&&)					= delete;
									D3D11Renderer& operator=(D3D11Renderer&&)		= delete;

public:
	bool							Initialise(Scene& scene)override;
	void							Update(float dt)override;
	void							Render()override;

	bool							CreateDevice()override;
	bool							CreateResources()override;
	bool							OnDeviceLost()override;

	D3DDevice*						GetDevice()override { return m_device.Get(); }
	D3DContext*						GetDeviceContext()override { return m_context.Get(); }
	Camera*							GetCamera() { return m_camera.get(); }

protected:
	void							InitializeInputLayout();

protected:
	UINT							m_buffersCount;
	UINT							m_msaa4xQuality;
	bool							m_enable4xMSAA;
	std::shared_ptr<Camera>			m_camera;
	Scene*							m_scene;

	CD3D11_VIEWPORT					m_viewport;
	D3D_DRIVER_TYPE					m_driverType;
	D3D_FEATURE_LEVEL				m_featureLevel;

	ComPtr<D3DDevice>				m_device;
	ComPtr<D3DContext>				m_context;
	ComPtr<IDXGISwapChain1>			m_swapChain;
	ComPtr<ID3D11RenderTargetView>	m_renderTargetView;
	ComPtr<ID3D11DepthStencilView>	m_depthStencilView;
	ComPtr<ID3D11RasterizerState>	m_rsState;

	ConstBuffer<CameraConstants>	m_constBuffer;
	ComPtr<ID3D11InputLayout>		m_inputLayout;
	ComPtr<ID3D11VertexShader>		m_vertexShader;
	ComPtr<ID3D11PixelShader>		m_pixelShader;

};

#endif //_RENDERER_H_
