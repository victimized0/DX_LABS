#include "pch.h"
#include "D3D11Renderer.h"
#include "SceneObjects/GeometryObject.h"
#include "Engine.h"

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
{

}

D3D11Renderer::~D3D11Renderer() {

}

bool D3D11Renderer::Initialise() {
	if (!CreateDevice()) {
		return false;
	}
	
	D3DRSDesc desc = {};
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_SOLID;
	if (CreateRSState(&desc) == RES_FAILED) {
		return false;
	}

	return true;
}

void D3D11Renderer::Render() {
	constexpr FLOAT backColour[3] = { 0.0f, 0.0f, 0.0f };

	m_context->ClearRenderTargetView(m_renderTargetView.Get(), backColour);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	for each ( auto& object in Engine::GetPtr()->GetScene().GetSceneObjects()) {
		auto geoObj = dynamic_cast<GeometryObject*>(object.get());
		if (geoObj != nullptr) {
			int rsStateId		= geoObj->GetRSStateId();
			int inputLayoutId	= geoObj->GetInputLayoutId();
			if (rsStateId < 0 || inputLayoutId < 0) {
				continue;
			}

			auto rsState = m_rsStates[rsStateId].Get();
			auto inputLayout = m_inputLayouts[inputLayoutId].Get();
			if (rsState == nullptr || inputLayout == nullptr) {
				continue;
			}

			m_context->RSSetState(rsState);
			m_context->IASetInputLayout(inputLayout);
			m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			int vsId = geoObj->GetVertexShaderId();
			int psId = geoObj->GetPixelShaderId();
			if (vsId < 0 || psId < 0) {
				continue;
			}

			auto vertexShader = m_vertexShaders[vsId].Get();
			auto pixelShader  = m_pixelShaders[psId].Get();
			if (vertexShader == nullptr || pixelShader == nullptr) {
				continue;
			}

			m_context->VSSetShader(vertexShader, nullptr, 0);
			m_context->PSSetShader(pixelShader,  nullptr, 0);

			int vbId = geoObj->GetVertexBufferId();
			int ibId = geoObj->GetIndexBufferId();
			if (vbId < 0 || ibId < 0) {
				continue;
			}

			auto vBuffer = m_vertexBuffers[vbId].Get();
			auto iBuffer = m_indexBuffers[ibId].Get();
			
			auto& camera = Engine::GetPtr()->GetScene().GetMainCamera();
			auto cBuffer = geoObj->GetConstBuffer(m_context.Get(), camera.GetView(), camera.GetProj());
			if (vBuffer == nullptr || iBuffer == nullptr || cBuffer == nullptr) {
				continue;
			}

			UINT stride = sizeof(GeometryObject::VertexType);
			UINT offset = 0;

			m_context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
			m_context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, offset);
			m_context->VSSetConstantBuffers(0, 1, &cBuffer);
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

	m_viewport.TopLeftX		= 0.0f;
	m_viewport.TopLeftY		= 0.0f;
	m_viewport.Width		= static_cast<float>(backBufferDesc.Width);
	m_viewport.Height		= static_cast<float>(backBufferDesc.Height);
	m_viewport.MinDepth		= D3D11_MIN_DEPTH;
	m_viewport.MaxDepth		= D3D11_MAX_DEPTH;
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

int D3D11Renderer::CreateVertexBuffer(size_t size, size_t strideSize, const void* pData) {
	VertexBuffer* pBuffer;
	if (FAILED(CreateBuffer(size, strideSize, pData, D3D11_BIND_VERTEX_BUFFER, &pBuffer))) {
		return RES_FAILED;
	}
	m_vertexBuffers.push_back(pBuffer);
	return m_vertexBuffers.size() - 1;
}

int D3D11Renderer::CreateIndexBuffer(size_t size, const void* pData) {
	IndexBuffer* pBuffer;
	if (FAILED(CreateBuffer(size, sizeof(UINT), pData, D3D11_BIND_INDEX_BUFFER, &pBuffer))) {
		return RES_FAILED;
	}
	m_indexBuffers.push_back(pBuffer);
	return m_indexBuffers.size() - 1;
}

int	D3D11Renderer::CreateInputLayout(InputElementDesc* desc, size_t arrSize, D3DBlob* shaderBlob) {
	InputLayout* pInputLayout;
	if (FAILED(m_device->CreateInputLayout(desc, arrSize, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &pInputLayout))) {
		return RES_FAILED;
	}
	m_inputLayouts.push_back(pInputLayout);
	return m_inputLayouts.size() - 1;
}

int D3D11Renderer::CreateRSState(D3DRSDesc* desc) {
	RSState* pRSState;
	if (FAILED(m_device->CreateRasterizerState(desc, &pRSState))) {
		return RES_FAILED;
	}
	m_rsStates.push_back(pRSState);
	return m_rsStates.size() - 1;
}

int D3D11Renderer::CreateVertexShader(const char* path, D3DBlob** ppBlob) {
	if (FAILED(CreateBlob(path, ppBlob))) {
		return RES_FAILED;
	}

	VertexShader* pVertexShader;
	ThrowIfFailed(m_device->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), nullptr, &pVertexShader));
	m_vertexShaders.push_back(pVertexShader);
	return m_vertexShaders.size() - 1;
}

int D3D11Renderer::CreatePixelShader(const char* path) {
	D3DBlob* psByteCode;
	if (FAILED(CreateBlob(path, &psByteCode))) {
		return RES_FAILED;
	}

	PixelShader* pPixelShader;
	ThrowIfFailed(m_device->CreatePixelShader(psByteCode->GetBufferPointer(), psByteCode->GetBufferSize(), nullptr, &pPixelShader));
	m_pixelShaders.push_back(pPixelShader);
	return m_pixelShaders.size() - 1;
}

HRESULT D3D11Renderer::CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, D3DBuffer** pBuffer) {
	assert(pBuffer != nullptr);

	uint64_t sizeInBytes = uint64_t((UINT)size * (UINT)strideSize);
	if (sizeInBytes > uint64_t(D3D11_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM * 1024u * 1024u)) {
		throw std::exception("Buffer too large for DirectX 11");
	}

	D3DBufferDesc desc = {};
	desc.ByteWidth		= static_cast<UINT>(sizeInBytes);
	desc.Usage			= D3D11_USAGE_DEFAULT;
	desc.BindFlags		= bindFlag;
	desc.CPUAccessFlags	= 0;

	D3DSubresData data = {};
	data.pSysMem = pData;

	return m_device->CreateBuffer(&desc, &data, pBuffer);
}

HRESULT D3D11Renderer::CreateBlob(const char* path, D3DBlob** pBlob) {
	std::string sPath(path);
	std::wstring wPath(sPath.begin(), sPath.end());
	std::wstring fullPath = Environment::Instance().GetExecPath() + wPath;
	
	return D3DReadFileToBlob(fullPath.c_str(), pBlob);
}
