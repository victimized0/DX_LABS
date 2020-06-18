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

float4 HDRPostProcessPS(in QuadOut input) : SV_Target {
	float3 hdr			= t_hdr.Sample(t_sampler, input.TexCoord).rgb;
	float3 cBloom		= t_bloom.Sample(t_sampler, input.TexCoord).rgb;
	float bloomAmount	= t_bloom.Sample(t_sampler, input.TexCoord).a;
	float3 bloom		= cBloom * bloomAmount;

	float3 ldr = TonemapFilmic(hdr + bloom);
	return float4(GammaCorrection(ldr), 1.0f);
}

float4 BloomPS(in QuadOut input) : SV_Target{
	float4 vColor = 0;
#ifdef USE_BLOOM
	float4 hdr = t_hdr.Sample(t_sampler, input.TexCoord);
	//if (hdr.rgb >= float3(BloomThreshold, BloomThreshold, BloomThreshold)) {
		vColor = hdr;
	//}
#endif // USE_BLOOM
	return vColor;
}

float4 HorizontalBlurPS(in QuadOut input) : SV_Target {
	// Create the weights that each neighbor pixel will contribute to the blur.
	float weight0 = 1.0f;
	float weight1 = 0.9f;
	float weight2 = 0.55f;
	float weight3 = 0.18f;
	float weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine horizontal pixels to the color by the specific weight of each.
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight4;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight3;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight2;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight1;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight0;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight1;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight2;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight3;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight4;

	// Set the alpha channel to one.
	color.a = 1.0f;
	return color;
}

float4 VerticalBlurPS(in QuadOut input) : SV_Target {
	// Create the weights that each neighbor pixel will contribute to the blur.
	float weight0 = 1.0f;
	float weight1 = 0.9f;
	float weight2 = 0.55f;
	float weight3 = 0.18f;
	float weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine vertical pixels to the color by the specific weight of each.
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight4;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight3;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight2;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight1;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight0;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight1;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight2;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight3;
	color += t_quadHdr.Sample(t_sampler, input.TexCoord) * weight4;

	// Set the alpha channel to one.
	color.a = 1.0f;
	return color;
}

