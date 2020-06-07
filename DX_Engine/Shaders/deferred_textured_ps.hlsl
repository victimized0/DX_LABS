#include "common.hlsli"

GBuffer main(in VS_PosColNmlTex input) {
	GBuffer OUT = (GBuffer)0;

	OUT.DiffuseAcc		= t_diffuse.Sample(t_sampler, input.TexCoord.xy);
#ifdef USE_LIGHT
	OUT.SpecularAcc.rgb	= (1.0f - t_specular.Sample(t_sampler, input.TexCoord.xy).rgb) * SpecularColor.rgb;
	OUT.SpecularAcc.a	= SpecularColor.a;
#endif
	OUT.NormalAcc		= float4(input.Normal, 1.0f);
	OUT.PositionAcc		= float4(input.WPos, 1.0f);

	return OUT;
}
