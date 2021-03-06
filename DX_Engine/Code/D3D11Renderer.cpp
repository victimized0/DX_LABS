#include "pch.h"
#include "D3D11Renderer.h"
#include "Engine.h"

#ifdef USE_DX

IRenderer* IRenderer::Create() {
	return new D3D11Renderer();
}

D3D11Renderer::D3D11Renderer() 
	: m_device(nullptr)
	, m_context(nullptr)
	, m_swapChain(nullptr)
	, m_backBuffer(nullptr)
	, m_depthStencilView(nullptr)
	, m_SRVs(4)
	, m_RTVs(4)
{
	m_backColour = { 0.0f, 0.0f, 0.0f };
}

D3D11Renderer::~D3D11Renderer() {

}

bool D3D11Renderer::Initialise() {
	if (!CreateDevice()) {
		return false;
	}
		
	Engine::GetPtr()->GetScene().Initialise( m_device.Get() );
	m_shadersManager.Initialise( m_device.Get() );

	D3D11_RASTERIZER_DESC rsBackSolidDesc	= {};
	rsBackSolidDesc.CullMode				= D3D11_CULL_BACK;
	rsBackSolidDesc.FillMode				= D3D11_FILL_SOLID;

	D3D11_RASTERIZER_DESC rsFrontSolid		= {};
	rsFrontSolid.CullMode					= D3D11_CULL_FRONT;
	rsFrontSolid.FillMode					= D3D11_FILL_SOLID;

	ThrowIfFailed(
		m_device->CreateRasterizerState(&rsBackSolidDesc, &m_rsCullBack)
	);

	ThrowIfFailed(
		m_device->CreateRasterizerState(&rsFrontSolid, &m_rsCullFront)
	);

	D3D11_SAMPLER_DESC samplerDesc	= {};
	samplerDesc.Filter				= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias			= 0.0f;
	samplerDesc.MaxAnisotropy		= 4;
	samplerDesc.ComparisonFunc		= D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD				= -FLT_MAX;
	samplerDesc.MaxLOD				= FLT_MAX;

	ThrowIfFailed(gEnv.Renderer()->GetDevice()->CreateSamplerState(&samplerDesc, &m_defaultSampler));
	m_context->PSSetSamplers(0, 1, m_defaultSampler.GetAddressOf());	// Using a default sampler

	m_context->PSSetShaderResources(0, m_SRVs.size(), m_SRVs.data());
	m_context->OMSetRenderTargets(m_RTVs.size(), m_RTVs.data(), m_depthStencilView.Get());

#ifdef _DEBUG
	HRESULT hr = m_context->QueryInterface(__uuidof(m_annotations), reinterpret_cast<void**>(m_annotations.GetAddressOf()));
	if (FAILED(hr)) return false;
#endif

	return true;
}

DirectX::SimpleMath::Vector2 D3D11Renderer::GetScreenSize() {
	return DirectX::SimpleMath::Vector2(m_viewport.Width, m_viewport.Height);
}

void D3D11Renderer::ClearFrame() {
	BEGIN_EVENT(L"Clear Frame")

	const float backCol[4] = { m_backColour[0], m_backColour[1], m_backColour[2], 0.0f };
	const float black[4] = { 0.f, 0.f, 0.f, 0.0f };

	//m_context->RSSetState(m_rsCullBack.Get());
	m_context->ClearRenderTargetView(m_backBuffer.Get(), backCol);
	m_context->ClearRenderTargetView(m_hdrRTV.Get(), black);
	m_context->ClearRenderTargetView(m_quadHdrRTV.Get(), black);
	m_context->ClearRenderTargetView(m_bloomRTV.Get(), black);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	END_EVENT(L"Clear Frame")
}

void D3D11Renderer::ClearGBuffer() {
	BEGIN_EVENT(L"Clear G-Buffer")

	const float backCol[4] = { m_backColour[0], m_backColour[1], m_backColour[2], 0.0f };
	const float black[4] = { 0.f, 0.f, 0.f, 0.0f };

	m_context->ClearRenderTargetView(m_sceneDiffuseAccRTV.Get(), backCol);
	m_context->ClearRenderTargetView(m_sceneSpecularAccRTV.Get(), black);
	m_context->ClearRenderTargetView(m_sceneNormalAccRTV.Get(), black);
	m_context->ClearRenderTargetView(m_scenePositionAccRTV.Get(), black);

	END_EVENT(L"Clear G-Buffer")
}

void D3D11Renderer::UnbindSRVs() {
	m_SRVs[0] = nullptr;
	m_SRVs[1] = nullptr;
	m_SRVs[2] = nullptr;
	m_SRVs[3] = nullptr;
	UpdateSRVs();
}

void D3D11Renderer::UnbindRTVs() {
	m_RTVs[0] = nullptr;
	m_RTVs[1] = nullptr;
	m_RTVs[2] = nullptr;
	m_RTVs[3] = nullptr;
	UpdateRTVs(nullptr);
}

void D3D11Renderer::UpdateSRVs() {
	m_context->PSSetShaderResources(0, m_SRVs.size(), m_SRVs.data());
}

void D3D11Renderer::UpdateRTVs(IDepthStencilView* depthView) {
	m_context->OMSetRenderTargets(m_RTVs.size(), m_RTVs.data(), depthView);
}

void D3D11Renderer::SetRSState(RastState rs) {
	if (rs == RastState::CULL_BACK_SOLID)
	{
		m_context->RSSetState(m_rsCullBack.Get());
	}
	else
	if (rs == RastState::CULL_FRONT_SOLID)
	{
		m_context->RSSetState(m_rsCullFront.Get());
	}
	else
	{
		// TODO: Implement
	}
}

void D3D11Renderer::SetBackColor(float r, float g, float b) {
	m_backColour = { r, g, b };
}

void D3D11Renderer::Render() {
	BEGIN_EVENT(L"Geometry pass")

	m_RTVs[0] = m_sceneDiffuseAccRTV.Get();
	m_RTVs[1] = m_sceneSpecularAccRTV.Get();
	m_RTVs[2] = m_sceneNormalAccRTV.Get();
	m_RTVs[3] = m_scenePositionAccRTV.Get();

	m_context->OMSetDepthStencilState(m_defaultDSState.Get(), 1);
	UpdateRTVs(m_depthStencilView.Get());
	Engine::GetPtr()->GetScene().RenderScene( m_context.Get(), RenderPass::Geometry );
	UnbindRTVs();

	END_EVENT(L"Geometry pass")
	BEGIN_EVENT(L"Light pass")

	const float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_SRVs[0] = m_sceneDiffuseSRV.Get();
	m_SRVs[1] = m_sceneSpecularSRV.Get();
	m_SRVs[2] = m_sceneNormalSRV.Get();
	m_SRVs[3] = m_scenePositionSRV.Get();
	m_RTVs[0] = m_hdrRTV.Get();

	m_context->IASetInputLayout(nullptr);
	m_context->IASetVertexBuffers(0, 0, nullptr, 0, 0);
	m_context->VSSetShader(m_shadersManager.FullscreenQuadVS.GetShader(), nullptr, 0);
	m_context->PSSetShader(m_shadersManager.BlinnPhongDeferredPS.GetShader(), nullptr, 0);
	UpdateSRVs();
	UpdateRTVs(m_depthStencilView.Get());
	m_context->OMSetDepthStencilState(m_readDSS.Get(), 1);
	m_context->OMSetBlendState(m_oneAddBS.Get(), blendFactor, 0xffffffff);
	
	Engine::GetPtr()->GetScene().RenderScene( m_context.Get(), RenderPass::Light );
	//ClearFrame();
	UnbindSRVs();
	UnbindRTVs();

	END_EVENT(L"Light pass")
	BEGIN_EVENT(L"Post-Processing")

	m_context->OMSetDepthStencilState(m_noDSState.Get(), 1);
	m_context->OMSetBlendState(nullptr, 0, 0xffffffff);

	// Copy hdr -> quadHdr
	m_SRVs[0] = m_hdrSRV.Get();
	m_RTVs[0] = m_quadHdrRTV.Get();

	m_context->RSSetViewports(1, &m_quadViewport);
	m_context->VSSetShader(m_shadersManager.FullscreenQuadVS.GetShader(), nullptr, 0);
	m_context->PSSetShader(m_shadersManager.CopyTexToTexPS.GetShader(), nullptr, 0);
	UpdateSRVs();
	UpdateRTVs(nullptr);
	m_context->Draw(3, 0);

	UnbindRTVs();
	UnbindSRVs();

	BEGIN_EVENT(L"Bloom")

	m_SRVs[0] = m_quadHdrSRV.Get(); // bloom src
	m_RTVs[0] = m_bloomRTV.Get();

	m_context->PSSetShader(m_shadersManager.BloomPS.GetShader(), nullptr, 0);
	UpdateSRVs();
	UpdateRTVs(nullptr);
	m_context->Draw(3, 0);
	UnbindRTVs();
	UnbindSRVs();

	BEGIN_EVENT(L"Vertical blur")

	m_SRVs[0] = m_bloomSRV.Get();
	m_RTVs[0] = m_quadHdrRTV.Get();

	m_context->PSSetShader(m_shadersManager.VerticalBlurPS.GetShader(), nullptr, 0);
	UpdateSRVs();
	UpdateRTVs(nullptr);
	m_context->Draw(3, 0);

	UnbindRTVs();
	UnbindSRVs();

	END_EVENT(L"Vertical blur")
	BEGIN_EVENT(L"Horizontal blur")

	m_SRVs[0] = m_quadHdrSRV.Get();
	m_RTVs[0] = m_bloomRTV.Get();

	m_context->PSSetShader(m_shadersManager.HorizontalBlurPS.GetShader(), nullptr, 0);
	UpdateSRVs();
	UpdateRTVs(nullptr);
	m_context->Draw(3, 0);

	UnbindRTVs();
	UnbindSRVs();

	END_EVENT(L"Horizontal blur")
	END_EVENT(L"Bloom")
	BEGIN_EVENT(L"Tone mapping")

	// Tone map
	m_SRVs[0] = m_hdrSRV.Get();
	m_SRVs[1] = m_bloomSRV.Get();
	m_RTVs[0] = m_backBuffer.Get();

	m_context->RSSetViewports(1, &m_viewport);
	m_context->PSSetShader(m_shadersManager.HDRPostProcessPS.GetShader(), nullptr, 0);
	UpdateSRVs();
	UpdateRTVs(nullptr);
	m_context->Draw(3, 0);

	UnbindRTVs();
	UnbindSRVs();

	END_EVENT(L"Tone mapping")
	END_EVENT(L"Post-Processing")
}

void D3D11Renderer::BeginFrame() {
	ClearFrame();
	ClearGBuffer();
}

void D3D11Renderer::EndFrame() {
	m_swapChain->Present(1, 0);
}

bool D3D11Renderer::CreateDevice() {
	UINT creationFlags = 0;
#ifdef _DEBUG 
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
#ifdef _USE_D2D
	creationFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width						= gEnv.Width;
	swapChainDesc.BufferDesc.Height						= gEnv.Height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count						= 1;
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount							= 1;
	swapChainDesc.OutputWindow							= gEnv.HWnd;
	swapChainDesc.Windowed								= true;
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFailed(
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_context)
	);

	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_backBuffer));

	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	backBuffer->GetDesc(&backBufferDesc);

	CD3D11_TEXTURE2D_DESC depthStencilDesc	= {};
	depthStencilDesc.Format					= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Width					= gEnv.Width;
	depthStencilDesc.Height					= gEnv.Height;
	depthStencilDesc.ArraySize				= 1;
	depthStencilDesc.MipLevels				= 1;
	depthStencilDesc.SampleDesc.Count		= 1;
	depthStencilDesc.SampleDesc.Quality		= 0;
	depthStencilDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage					= D3D11_USAGE_DEFAULT;
	depthStencilDesc.CPUAccessFlags			= 0;
	depthStencilDesc.MiscFlags				= 0;

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

	m_quadViewport.TopLeftX = 0.0f;
	m_quadViewport.TopLeftY = 0.0f;
	m_quadViewport.Width	= static_cast<float>(backBufferDesc.Width / 4);
	m_quadViewport.Height	= static_cast<float>(backBufferDesc.Height / 4);
	m_quadViewport.MinDepth = D3D11_MIN_DEPTH;
	m_quadViewport.MaxDepth = D3D11_MAX_DEPTH;

	D3D11_TEXTURE2D_DESC textureDesc	= {};
	textureDesc.Width					= gEnv.Width;
	textureDesc.Height					= gEnv.Height;
	textureDesc.MipLevels				= 1;
	textureDesc.ArraySize				= 1;
	textureDesc.Format					= DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count		= 1;
	textureDesc.Usage					= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags				= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags			= 0;
	textureDesc.MiscFlags				= 0;

	D3D11_TEXTURE2D_DESC quadTexDesc	= textureDesc;
	quadTexDesc.Width					= textureDesc.Width / 4;
	quadTexDesc.Height					= textureDesc.Height / 4;

	D3D11_TEXTURE2D_DESC shadowTexDesc	= textureDesc;
	shadowTexDesc.Width					= 1024;
	shadowTexDesc.Height				= 1024;

	ComPtr<ID3D11Texture2D>	diffuseTex;
	ComPtr<ID3D11Texture2D>	specularTex;
	ComPtr<ID3D11Texture2D>	normalTex;
	ComPtr<ID3D11Texture2D>	positionTex;
	ComPtr<ID3D11Texture2D>	hdrTex;
	ComPtr<ID3D11Texture2D>	quadHdrTex;
	ComPtr<ID3D11Texture2D>	bloomTex;
	ComPtr<ID3D11Texture2D>	shadowMapTex;

	if (m_device->CreateTexture2D(&textureDesc, nullptr, &diffuseTex)		!= S_OK) return false;
	if (m_device->CreateTexture2D(&textureDesc, nullptr, &specularTex)		!= S_OK) return false;
	if (m_device->CreateTexture2D(&textureDesc, nullptr, &normalTex)		!= S_OK) return false;
	if (m_device->CreateTexture2D(&textureDesc, nullptr, &positionTex)		!= S_OK) return false;
	if (m_device->CreateTexture2D(&textureDesc, nullptr, &hdrTex)			!= S_OK) return false;
	if (m_device->CreateTexture2D(&quadTexDesc, nullptr, &quadHdrTex)		!= S_OK) return false;
	if (m_device->CreateTexture2D(&quadTexDesc, nullptr, &bloomTex)			!= S_OK) return false;
	if (m_device->CreateTexture2D(&shadowTexDesc, nullptr, &shadowMapTex)	!= S_OK) return false;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc	= {};
	rtvDesc.Format							= textureDesc.Format;
	rtvDesc.ViewDimension					= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice				= 0;

	// Create the G-Buffer RTVs
	if (m_device->CreateRenderTargetView(diffuseTex.Get(), &rtvDesc, &m_sceneDiffuseAccRTV)		!= S_OK) return false;
	if (m_device->CreateRenderTargetView(specularTex.Get(), &rtvDesc, &m_sceneSpecularAccRTV)	!= S_OK) return false;
	if (m_device->CreateRenderTargetView(normalTex.Get(), &rtvDesc, &m_sceneNormalAccRTV)		!= S_OK) return false;
	if (m_device->CreateRenderTargetView(positionTex.Get(), &rtvDesc, &m_scenePositionAccRTV)	!= S_OK) return false;
	if (m_device->CreateRenderTargetView(hdrTex.Get(), &rtvDesc, &m_hdrRTV)						!= S_OK) return false;
	if (m_device->CreateRenderTargetView(quadHdrTex.Get(), &rtvDesc, &m_quadHdrRTV)				!= S_OK) return false;
	if (m_device->CreateRenderTargetView(bloomTex.Get(), &rtvDesc, &m_bloomRTV)					!= S_OK) return false;
	if (m_device->CreateRenderTargetView(shadowMapTex.Get(), &rtvDesc, &m_shadowMapRTV)			!= S_OK) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format							= textureDesc.Format;
	srvDesc.ViewDimension					= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip		= 0;
	srvDesc.Texture2D.MipLevels				= 1;

	if (m_device->CreateShaderResourceView(diffuseTex.Get(), &srvDesc, &m_sceneDiffuseSRV)		!= S_OK) return false;
	if (m_device->CreateShaderResourceView(specularTex.Get(), &srvDesc, &m_sceneSpecularSRV)	!= S_OK) return false;
	if (m_device->CreateShaderResourceView(normalTex.Get(), &srvDesc, &m_sceneNormalSRV)		!= S_OK) return false;
	if (m_device->CreateShaderResourceView(positionTex.Get(), &srvDesc, &m_scenePositionSRV)	!= S_OK) return false;
	if (m_device->CreateShaderResourceView(hdrTex.Get(), &srvDesc, &m_hdrSRV)					!= S_OK) return false;
	if (m_device->CreateShaderResourceView(quadHdrTex.Get(), &srvDesc, &m_quadHdrSRV)			!= S_OK) return false;
	if (m_device->CreateShaderResourceView(bloomTex.Get(), &srvDesc, &m_bloomSRV)				!= S_OK) return false;
	if (m_device->CreateShaderResourceView(shadowMapTex.Get(), &srvDesc, &m_shadowMapSRV)		!= S_OK) return false;
	
	D3D11_DEPTH_STENCIL_DESC dsDesc			= {};
	dsDesc.DepthEnable						= true;
	dsDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable					= false;
	dsDesc.StencilReadMask					= 0xFF;
	dsDesc.StencilWriteMask					= 0xFF;
	dsDesc.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp		= D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp		= D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc				= D3D11_COMPARISON_ALWAYS;

	if (m_device->CreateDepthStencilState(&dsDesc, &m_defaultDSState) != S_OK) return false;

	D3D11_DEPTH_STENCIL_DESC noDSDesc		= {};
	noDSDesc.DepthEnable					= false;
	noDSDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ZERO;
	noDSDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	noDSDesc.StencilEnable					= false;
	noDSDesc.StencilReadMask				= 0xFF;
	noDSDesc.StencilWriteMask				= 0xFF;
	noDSDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	noDSDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	noDSDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	noDSDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	noDSDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	noDSDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	noDSDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	noDSDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	if (m_device->CreateDepthStencilState(&noDSDesc, &m_noDSState) != S_OK) return false;

	D3D11_DEPTH_STENCIL_DESC readDSDesc		= {};
	readDSDesc.DepthEnable					= true;
	readDSDesc.DepthWriteMask				= D3D11_DEPTH_WRITE_MASK_ZERO;
	readDSDesc.DepthFunc					= D3D11_COMPARISON_LESS;
	readDSDesc.StencilEnable				= false;
	readDSDesc.StencilReadMask				= 0xFF;
	readDSDesc.StencilWriteMask				= 0xFF;
	readDSDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	readDSDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	readDSDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	readDSDesc.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;
	readDSDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	readDSDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	readDSDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	readDSDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	if (m_device->CreateDepthStencilState(&readDSDesc, &m_readDSS) != S_OK) return false;

	D3D11_BLEND_DESC oneAddBlendDesc						= {};
	oneAddBlendDesc.RenderTarget[0].BlendEnable				= true;
	oneAddBlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	oneAddBlendDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	oneAddBlendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_ONE;
	oneAddBlendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE;
	oneAddBlendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	oneAddBlendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	oneAddBlendDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;

	if (m_device->CreateBlendState(&oneAddBlendDesc, &m_oneAddBS) != S_OK) return false;

	return true;
}

bool D3D11Renderer::OnDeviceLost() {
	m_depthStencilView.Reset();
	m_backBuffer.Reset();
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

IDXGISurface* D3D11Renderer::GetBackBuffer() {
	IDXGISurface* pBackBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	return pBackBuffer;
}

#endif // USE_DX
