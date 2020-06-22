#include "pch.h"
#include "ShadowMapping.h"

ShadowMapping::ShadowMapping() {

}

ShadowMapping::~ShadowMapping() {

}

bool ShadowMapping::Initialise(IDevice* device) {
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

	if (device->CreateTexture2D(&textureDesc, nullptr, &m_texShadowMap) != S_OK) {
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc	= {};
	rtvDesc.Format							= textureDesc.Format;
	rtvDesc.ViewDimension					= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice				= 0;

	if (device->CreateRenderTargetView(m_texShadowMap.Get(), &rtvDesc, &m_rtvShadowMap) != S_OK) {
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format							= textureDesc.Format;
	srvDesc.ViewDimension					= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip		= 0;
	srvDesc.Texture2D.MipLevels				= 1;

	if (device->CreateShaderResourceView(m_texShadowMap.Get(), &srvDesc, &m_srvShadowMap) != S_OK) {
		return false;
	}

	m_pVertexShader = &gEnv.Renderer()->GetShadersManager()->StandardNoLightVS;
	m_pPixelShader = &gEnv.Renderer()->GetShadersManager()->StandardNoLightPS;
}
