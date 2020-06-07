#include "common.hlsli"

struct QuadOut {
	float4 Position : SV_Position;
	float2 TexCoord : TEXCOORD;
};

float3 GammaCorrection(in float3 color) {
	return pow(color, 2.2f);
}

float3 TonemapFilmic(float3 x) {
	float3 X = max( float3(0.0f, 0.0f, 0.0f), x - 0.004f );
	float3 result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
	return result;
}

float TonemapFilmic(float x) {
	float X = max(0.0f, x - 0.004f);
	float result = (X * (6.2f * X + 0.5f)) / (X * (6.2f * X + 1.7f) + 0.06f);
	return result;
}

float4 HDRPostProcessPS(in QuadOut input) : SV_TARGET {
	float3 hdr = t_hdr.Sample(t_sampler, input.TexCoord).rgb;

	float3 ldr = TonemapFilmic(hdr);
	return float4(GammaCorrection(ldr), 1.0f);
}
