#include "pch.h"
#include "ShadersManager.h"

ShadersManager::ShadersManager()
	: StandardNoLightVS("standard_vs")
	, StandardWithLightVS("standard_vs")
	, TexturedNoLightVS("textured_vs")
	, TexturedWithLightVS("textured_vs")
	, FullscreenQuadVS("deferred")

	, StandardNoLightPS("standard_ps")
	, StandardWithLightPS("standard_ps")
	, TexturedNoLightPS("textured_ps")
	, TexturedWithLightPS("textured_ps")
	, DeferredTexturedNoLightPS("deferred_textured_ps")
	, DeferredTexturedWithLightPS("deferred_textured_ps")
	, BlinnPhongDeferredPS("deferred")
	, PointLightPS("deferred")
	, HDRPostProcessPS("HDR")
	, BloomPS("HDR")
	, HorizontalBlurPS("HDR")
	, VerticalBlurPS("HDR")
	, CopyTexToTexPS("standard_ps")
{

}

void ShadersManager::Initialise(IDevice* device) {
	std::vector<D3DShaderMacro> lightMacros = {
		{ "USE_LIGHT", "1" },
		{ NULL, NULL }
	};

	std::vector<D3DShaderMacro> bloomMacros = {
		{ "USE_BLOOM", "1" },
		{ "USE_LIGHT", "1" },
		{ NULL, NULL }
	};

	std::vector<D3DShaderMacro> emptyMacros = {
		{ NULL, NULL }
	};

	StandardNoLightVS	.Create(device, "main", emptyMacros);
	StandardWithLightVS	.Create(device, "main", lightMacros);
	TexturedNoLightVS	.Create(device, "main", emptyMacros);
	TexturedWithLightVS	.Create(device, "main", lightMacros);
	FullscreenQuadVS	.Create(device, "FullscreenQuadVS", lightMacros);

	StandardNoLightPS			.Create(device, "main", emptyMacros);
	StandardWithLightPS			.Create(device, "main", lightMacros);
	TexturedNoLightPS			.Create(device, "main", emptyMacros);
	TexturedWithLightPS			.Create(device, "main", lightMacros);
	DeferredTexturedNoLightPS	.Create(device, "main", emptyMacros);
	DeferredTexturedWithLightPS	.Create(device, "main", lightMacros);
	BlinnPhongDeferredPS		.Create(device, "BlinnPhongDeferredPS", lightMacros);
	PointLightPS				.Create(device, "PointLightPS", lightMacros);
	HDRPostProcessPS			.Create(device, "HDRPostProcessPS", lightMacros);
	BloomPS						.Create(device, "BloomPS", bloomMacros);
	HorizontalBlurPS			.Create(device, "HorizontalBlurPS", bloomMacros);
	VerticalBlurPS				.Create(device, "VerticalBlurPS", bloomMacros);
	CopyTexToTexPS				.Create(device, "CopyTexToTexPS", emptyMacros);
}
