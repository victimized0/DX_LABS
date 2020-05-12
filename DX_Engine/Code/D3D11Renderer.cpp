#include "pch.h"
#include "D3D11Renderer.h"

#include "Engine.h"
#include "Shader.h"

#ifdef USE_DX

IRenderer* IRenderer::Create() {
	return new D3D11Renderer();
}

D3D11Renderer::D3D11Renderer() 
	: m_device(nullptr)
	, m_context(nullptr)
	, m_swapChain(nullptr)
	, m_renderTargetView(nullptr)
	, m_depthStencilView(nullptr)
	, m_pDirLight(nullptr)
{
	m_backColour = { 0.0f, 0.0f, 0.0f };
}

D3D11Renderer::~D3D11Renderer() {

}

bool D3D11Renderer::Initialise() {
	if (!CreateDevice()) {
		return false;
	}

	CreateResources();

	m_cbPerFrame.Create(m_device.Get());

	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FillMode = D3D11_FILL_SOLID;

	ThrowIfFailed(
		m_device->CreateRasterizerState(&rsDesc, &m_defaultRSState)
	);

	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	return true;
}

void D3D11Renderer::CreateResources() {
	D3DShaderMacro lightMacro = { "USE_DIR_LIGHT", "1" };
	D3DShaderMacro emptyMacro = { NULL, NULL };

	const D3DShaderMacro macrosLight[2] =
	{
		lightMacro,
		emptyMacro
	};

	auto pStandardVS = std::make_unique<VertexShader>( SV_STANDARD, nullptr, "main" );
	auto pStandardPS = std::make_unique<PixelShader> ( SP_STANDARD, nullptr, "main" );

	auto pTexturedVSNoLight = std::make_unique<VertexShader>( SV_TEXTURED, nullptr, "main" );
	auto pTexturedPSNoLight = std::make_unique<PixelShader> ( SP_TEXTURED, nullptr, "main" );

	auto pTexturedVSWithLight = std::make_unique<VertexShader>( SV_TEXTURED, macrosLight, "main" );
	auto pTexturedPSWithLight = std::make_unique<PixelShader> ( SP_TEXTURED, macrosLight, "main" );

	gEnv.ResManager()->CreateResource( std::move( pStandardVS ) );
	gEnv.ResManager()->CreateResource( std::move( pStandardPS ) );

	gEnv.ResManager()->CreateResource( std::move( pTexturedVSNoLight ) );
	gEnv.ResManager()->CreateResource( std::move( pTexturedPSNoLight ) );

	gEnv.ResManager()->CreateResource( std::move( pTexturedVSWithLight ) );
	gEnv.ResManager()->CreateResource( std::move( pTexturedPSWithLight ) );
}

void D3D11Renderer::ClearFrame() {
	const float backCol[3] = { m_backColour[0], m_backColour[1], m_backColour[2] };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), backCol);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3D11Renderer::SetBackColor(float r, float g, float b) {
	m_backColour = { r, g, b };
}

void D3D11Renderer::SetSunLight(DirLight* pDirLight) {
	m_pDirLight = pDirLight;
}

void D3D11Renderer::Render() {
	ClearFrame();

	CBPerFrame cbpf = {};
	cbpf.EyePos = DirectX::SimpleMath::Vector4(Engine::GetPtr()->GetScene().GetMainCamera()->GetPosition(), 1.0f);

	if (m_pDirLight != nullptr) {
		cbpf.LightCol = m_pDirLight->LightCol;
		cbpf.LightAmb = m_pDirLight->LightAmb;
		cbpf.LightDir = DirectX::SimpleMath::Vector4(m_pDirLight->LightDir, 0.0f);
	}

	m_cbPerFrame.SetData(m_context.Get(), cbpf);
	IConstBuffer* cb = m_cbPerFrame.GetBuffer();

	m_context->VSSetConstantBuffers((UINT)CBPerFrame::Slot, 1, &cb);
	m_context->PSSetConstantBuffers((UINT)CBPerFrame::Slot, 1, &cb);

	m_context->RSSetState(m_defaultRSState.Get());

	Engine::GetPtr()->GetScene().RenderScene(m_context.Get());

	m_swapChain->Present(1, 0);
}

bool D3D11Renderer::CreateDevice() {
	UINT creationFlags = 0;
#ifdef _DEBUG 
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width						= gEnv.Width;
	swapChainDesc.BufferDesc.Height						= gEnv.Height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 0;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count						= 1;
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount							= 1;
	swapChainDesc.OutputWindow							= gEnv.HWnd;
	swapChainDesc.Windowed								= true;
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags									= 0;

	ThrowIfFailed(
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_context)
	);

	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView));

	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	backBuffer->GetDesc(&backBufferDesc);

	CD3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Width				= gEnv.Width;
	depthStencilDesc.Height				= gEnv.Height;
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.SampleDesc.Count	= 1;
	depthStencilDesc.SampleDesc.Quality	= 0;
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
	m_viewport.MinDepth = D3D11_MIN_DEPTH;
	m_viewport.MaxDepth = D3D11_MAX_DEPTH;
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
	return true;
}

HRES D3D11Renderer::CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, IBuffer** pBuffer) {
	assert(pBuffer != nullptr);

	uint64_t sizeInBytes = uint64_t(size * strideSize);
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

#endif //USE_DX
