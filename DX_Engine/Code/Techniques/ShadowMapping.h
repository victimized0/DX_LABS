#ifndef _SHADOW_MAPPING_H_
#define _SHADOW_MAPPING_H_
#pragma once

#include "../Interfaces/ITechnique.h"
#include "../Shader.h"
#include "../ConstBuffer.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class ShadowMapping final : public ITechnique {
public:
								ShadowMapping();
	virtual						~ShadowMapping();

	bool						Initialise(IDevice* device)final;
	void						Draw(IDevCon* context)final;

private:
	ConstBuffer<CBPerFrame>		m_cBufferData;

	ComPtr<ITexture2D>			m_texShadowMap;
	ComPtr<IRenderTargetView>	m_rtvShadowMap;
	ComPtr<IShaderResView>		m_srvShadowMap;

	VertexShader*				m_pVertexShader;
	PixelShader*				m_pPixelShader;

};

#endif //_SHADOW_MAPPING_H_
