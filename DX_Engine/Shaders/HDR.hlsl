#include "common.hlsli"

Texture2D t_bloomSrc : register(REG_TEX_DIFFUSE);
static const float g_Weights[5] = { 0.227027f, 0.194595f, 0.121622f, 0.054054f, 0.016216f };

float3 GammaCorrection(in float3 color) {
	return pow(color, 2.2f);
}

float3 ReinhardExtended(float3 v, float max_white) {
	float3 numerator = v * (1.0f + (v / (max_white * max_white)));
	return numerator / (1.0f + v);
}

float3 TonemapFilmic(float3 x) {
	float3 X = max( float3(0.0f, 0.0f, 0.0f), x - 0.004f );
	float3 result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
	return GammaCorrection(result);
}

float4 HDRPostProcessPS(in QuadOut input) : SV_Target {
	float3 hdr			= t_hdr.Sample(t_sampler, input.TexCoord).rgb;
	float3 cBloom		= t_bloom.Sample(t_sampler, input.TexCoord).rgb;
	//float bloomAmount	= t_bloom.Sample(t_sampler, input.TexCoord).a;
	//float3 bloom		= cBloom * bloomAmount;

	float3 ldr = TonemapFilmic(hdr + cBloom);
	//float3 ldr = ReinhardExtended(hdr + cBloom, 10.0f);
	return float4(ldr, 1.0f);
}

float4 BloomPS(in QuadOut input) : SV_Target {
	float4 vColor = 0;
#ifdef USE_BLOOM
	float4 hdr = t_quadHdr.Sample(t_sampler, input.TexCoord);
	if (length(hdr.rgb) > 2.5f) {
		vColor.rgb = hdr.rgb;
	}
#endif // USE_BLOOM
	return vColor;
}

float4 VerticalBlurPS(in QuadOut input) : SV_Target {
	float2 size = 0;
	t_bloomSrc.GetDimensions(size.x, size.y);

	float2 texOffset = 1.0 / size;
	float3 result = t_bloomSrc.Sample(t_sampler, input.TexCoord).rgb * g_Weights[0];

	[unroll]
	for (int i = 1; i < 5; ++i) {
		result += t_bloomSrc.Sample(t_sampler, input.TexCoord + float2(0.0f, texOffset.y * i)) * g_Weights[i];
		result += t_bloomSrc.Sample(t_sampler, input.TexCoord - float2(0.0f, texOffset.y * i)) * g_Weights[i];
	}

	return float4(result, 1.0f);
}

float4 HorizontalBlurPS(in QuadOut input) : SV_Target {
	float2 size = 0;
	t_bloomSrc.GetDimensions(size.x, size.y);

	float2 texOffset = 1.0 / size;
	float3 result = t_bloomSrc.Sample(t_sampler, input.TexCoord).rgb * g_Weights[0];

	[unroll]
	for (int i = 1; i < 5; ++i) {
		result += t_bloomSrc.Sample(t_sampler, input.TexCoord + float2(texOffset.x * i, 0.0f)) * g_Weights[i];
		result += t_bloomSrc.Sample(t_sampler, input.TexCoord - float2(texOffset.x * i, 0.0f)) * g_Weights[i];
	}

	return float4(result, 1.0f);
}

