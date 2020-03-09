#include "pch.h"
#include "D3D11Renderer.h"
#include "SceneObjects/GeometryObject.h"

D3D11Renderer::D3D11Renderer() 
	: m_msaa4xQuality(1)
	, m_buffersCount(1)
	, m_enable4xMSAA(false)
	, m_driverType(D3D_DRIVER_TYPE_HARDWARE)
	, m_featureLevel(D3D_FEATURE_LEVEL_11_0)

	, m_device(nullptr)
	, m_context(nullptr)
	, m_swapChain(nullptr)
	, m_renderTargetView(nullptr)
	, m_depthStencilView(nullptr)
	, m_rsState(nullptr)
{
	m_camera = std::make_shared<Camera>("Main Camera");
}

D3D11Renderer::~D3D11Renderer() {

}

bool D3D11Renderer::Initialise(Scene& scene) {
	if (!CreateDevice()) {
		return false;
	}

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;

	HRESULT hr = m_device->CreateRasterizerState(&rastDesc, &m_rsState);
	if (FAILED(hr)) {
		return false;
	}

	m_scene = &scene;
	m_scene->AddObject(m_camera);
	m_constBuffer.Create(m_device.Get());

	InitializeInputLayout();
	return true;
}

void D3D11Renderer::Update(float dt) {
	m_scene->Update(dt);
}

void D3D11Renderer::Render() {
	constexpr FLOAT backColour[3] = { 0.0f, 0.0f, 0.0f };

	m_context->ClearRenderTargetView(m_renderTargetView.Get(), backColour);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	m_context->RSSetState(m_rsState.Get());

	m_context->IASetInputLayout(m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	CameraConstants cameraConstants = {};
	XMStoreFloat4x4(&cameraConstants.worldViewProj, m_camera->GetView() * m_camera->GetProj());
	XMStoreFloat4x4(&cameraConstants.world, DirectX::XMMatrixIdentity());

	m_constBuffer.SetData(m_context.Get(), cameraConstants);
	auto cameraConstBuffer = m_constBuffer.GetBuffer();
	m_context->VSSetConstantBuffers(0, 1, &cameraConstBuffer);

	UINT stride = sizeof(SimpleVertexColour);
	UINT offset = 0;

	// TODO: Implement a proper way of setting shaders
	m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	for each ( auto& object in m_scene->GetSceneObjects()) {
		auto geoObj = dynamic_cast<GeometryObject*>(object.get());
		if (geoObj != nullptr) {
			geoObj->UpdateVertexBuffer(m_context.Get());

			auto vBuffer = geoObj->GetVertexBuffer();
			auto iBuffer = geoObj->GetIndexBuffer();

			m_context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
			m_context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, offset);
			m_context->DrawIndexed((UINT)geoObj->GetIndices().size(), 0, 0);
		}
	}

	m_swapChain->Present(1, 0);
}

bool D3D11Renderer::CreateDevice() {
	UINT creationFlags = 0;
#ifdef _DEBUG 
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	ComPtr<ID3D11Device>			d3dDevice;
	ComPtr<ID3D11DeviceContext>		d3dContext;
	HRESULT hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &d3dDevice, &m_featureLevel, &d3dContext);

	if (hr == E_INVALIDARG)
		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
		hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, creationFlags, &featureLevels[1], ARRAYSIZE(featureLevels) - 1, D3D11_SDK_VERSION, &d3dDevice, &m_featureLevel, &d3dContext);

	ThrowIfFailed(hr);

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(d3dDevice.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif	//	_DEBUG
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif	//	!NDEBUG

	ThrowIfFailed(d3dDevice.As(&m_device));
	ThrowIfFailed(d3dContext.As(&m_context));

	return true;
}

bool D3D11Renderer::CreateResources() {
#ifdef _DEBUG
	assert(m_device);
	assert(m_context);
#endif

	ThrowIfFailed(m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_msaa4xQuality));
	if (m_msaa4xQuality >= 4) {
		m_enable4xMSAA = true;
	}

	if (m_swapChain) {
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_context->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
		m_renderTargetView.Reset();
		m_depthStencilView.Reset();
		m_context->Flush();

		HRESULT hr = m_swapChain->ResizeBuffers(m_buffersCount,
												Environment::Instance().GetWidth(),
												Environment::Instance().GetHeight(),
												DXGI_FORMAT_B8G8R8A8_UNORM,
												DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
			if (!OnDeviceLost()) {
				return false;
			}
		} else {
			ThrowIfFailed(hr);
		}
	} else {
		ComPtr<IDXGISwapChain> swapChain;
		ComPtr<IDXGIDevice2> dxgiDevice;
		ThrowIfFailed(m_device.As(&dxgiDevice));

		ComPtr<IDXGIAdapter> dxgiAdapter;
		ThrowIfFailed(dxgiDevice->GetAdapter(&dxgiAdapter));

		ComPtr<IDXGIFactory1> dxgiFactory;
		ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (dxgiFactory.As(&dxgiFactory2)) {
			// DirectX 11.1 or later
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.Width						= Environment::Instance().GetWidth();
			swapChainDesc.Height					= Environment::Instance().GetHeight();
			swapChainDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.SampleDesc.Count			= m_enable4xMSAA ? 4 : 1;
			swapChainDesc.SampleDesc.Quality		= m_enable4xMSAA ? (m_msaa4xQuality - 1) : 0;
			swapChainDesc.BufferUsage				= DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount				= m_buffersCount;
			swapChainDesc.SwapEffect				= DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags						= 0;
			swapChainDesc.Stereo					= false;
			swapChainDesc.Scaling					= DXGI_SCALING_NONE;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC sd_fullscreen = {};
			sd_fullscreen.RefreshRate.Numerator		= 60;
			sd_fullscreen.RefreshRate.Denominator	= 1;
			sd_fullscreen.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd_fullscreen.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
			sd_fullscreen.Windowed					= !Environment::Instance().GetIsFullscreen();

			ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(	m_device.Get(),
																Environment::Instance().GetWindowHandle(),
																&swapChainDesc,
																&sd_fullscreen,
																nullptr,
																&m_swapChain));
		} else {
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferDesc.Width						= Environment::Instance().GetWidth();
			swapChainDesc.BufferDesc.Height						= Environment::Instance().GetHeight();
			swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
			swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
			swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.SampleDesc.Count						= m_enable4xMSAA ? 4 : 1;
			swapChainDesc.SampleDesc.Quality					= m_enable4xMSAA ? (m_msaa4xQuality - 1) : 0;
			swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount							= m_buffersCount;
			swapChainDesc.OutputWindow							= Environment::Instance().GetWindowHandle();
			swapChainDesc.Windowed								= !Environment::Instance().GetIsFullscreen();
			swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			ThrowIfFailed(dxgiFactory->CreateSwapChain(m_device.Get(), &swapChainDesc, &swapChain));
			ThrowIfFailed(swapChain.As(&m_swapChain));
		}

		ThrowIfFailed(dxgiFactory->MakeWindowAssociation(Environment::Instance().GetWindowHandle(), DXGI_MWA_NO_WINDOW_CHANGES));
	}

	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView));

	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	backBuffer->GetDesc(&backBufferDesc);

	CD3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Width				= Environment::Instance().GetWidth();
	depthStencilDesc.Height				= Environment::Instance().GetHeight();
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.SampleDesc.Count	= m_enable4xMSAA ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality	= m_enable4xMSAA ? (m_msaa4xQuality - 1) : 0;
	depthStencilDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthStencilDesc.CPUAccessFlags		= 0;
	depthStencilDesc.MiscFlags			= 0;

	ComPtr<ID3D11Texture2D> depthStencil;
	ThrowIfFailed(m_device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil));
	ThrowIfFailed(m_device->CreateDepthStencilView(depthStencil.Get(), nullptr, &m_depthStencilView));

	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width	= static_cast<float>(backBufferDesc.Width);
	m_viewport.Height	= static_cast<float>(backBufferDesc.Height);
	m_viewport.MinDepth	= D3D11_MIN_DEPTH;
	m_viewport.MaxDepth	= D3D11_MAX_DEPTH;
	m_context->RSSetViewports(1, &m_viewport);

	return true;
}

bool D3D11Renderer::OnDeviceLost() {
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_context.Reset();
	m_device.Reset();

	CreateDevice();
	CreateResources();

	return true;
}

void D3D11Renderer::InitializeInputLayout() {
	ID3DBlob* vsbyteCode = nullptr;
	ID3DBlob* psbyteCode = nullptr;

	HRESULT result = D3DReadFileToBlob((Environment::Instance().GetExecPath() + L"\\data\\shaders\\standard_vs.cso").c_str(), &vsbyteCode);
	if (FAILED(result)) {
		return;
	}

	result = D3DReadFileToBlob((Environment::Instance().GetExecPath() + L"\\data\\shaders\\standard_ps.cso").c_str(), &psbyteCode);
	if (FAILED(result)) {
		return;
	}

	ThrowIfFailed(m_device->CreateVertexShader(vsbyteCode->GetBufferPointer(), vsbyteCode->GetBufferSize(), nullptr, &m_vertexShader));
	ThrowIfFailed(m_device->CreatePixelShader(psbyteCode->GetBufferPointer(), psbyteCode->GetBufferSize(), nullptr, &m_pixelShader));

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), vsbyteCode->GetBufferPointer(), vsbyteCode->GetBufferSize(), &m_inputLayout);
}
