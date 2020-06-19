#include "common.hlsli"

GBuffer main(in VS_PosColNmlTex input) {
	GBuffer OUT = (GBuffer)0;

	OUT.DiffuseAcc		= input.Color;
	OUT.SpecularAcc.rgb = 0.1f;
	OUT.SpecularAcc.a	= 1.0f;
	OUT.NormalAcc		= float4(input.Normal, 1.0f);
	OUT.PositionAcc		= float4(input.WPos, 1.0f);

	return OUT;
}

float4 CopyTexToTexPS(in QuadOut input) : SV_Target {
	float3 color = t_src.Sample(t_sampler, input.TexCoord).rgb;
	return float4(color, 1.0f);
}
