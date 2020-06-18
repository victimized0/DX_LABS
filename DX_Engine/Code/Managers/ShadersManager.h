#ifndef	_SHADERS_MANAGER_H_
#define	_SHADERS_MANAGER_H_
#pragma once

#include "../Shader.h"

class ShadersManager {
public:
					ShadersManager();
					~ShadersManager() = default;

	void			Initialise(IDevice* device);

public:
	VertexShader	StandardNoLightVS;
	VertexShader	StandardWithLightVS;
	VertexShader	TexturedNoLightVS;
	VertexShader	TexturedWithLightVS;
	VertexShader	FullscreenQuadVS;

	PixelShader		StandardNoLightPS;
	PixelShader		StandardWithLightPS;
	PixelShader		TexturedNoLightPS;
	PixelShader		TexturedWithLightPS;
	PixelShader		DeferredTexturedNoLightPS;
	PixelShader		DeferredTexturedWithLightPS;
	PixelShader		BlinnPhongDeferredPS;
	PixelShader		HDRPostProcessPS;
	PixelShader		BloomPS;
	PixelShader		HorizontalBlurPS;
	PixelShader		VerticalBlurPS;
};

#endif // _SHADERS_MANAGER_H_
