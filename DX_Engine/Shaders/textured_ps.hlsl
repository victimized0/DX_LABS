#include "common.hlsli"

float4 main(in VS_PosColNmlTex input) : SV_TARGET {
	float4 diffuse	= t_diffuse.Sample(t_sampler, input.TexCoord.xy);
	float4 color	= diffuse;

#ifdef USE_LIGHT
	float4 specular = t_specular.Sample(t_sampler, input.TexCoord.xy);

	Material m;
	m.Ka = diffuse * AmbientColor.rgb;
	m.Kd = diffuse;//DiffuseColor.rgb;
	m.Ks = (1.0f - specular) * SpecularColor.rgb;
	m.A  = SpecularColor.a;

	float3 normal	= normalize( input.Normal.xyz );
	float3 lightDir	= normalize( LightDir.xyz );
	float3 V		= normalize( EyePos - (float3) input.Position );
	float3 h		= normalize( -lightDir + V );

	color = calcBlinnPhongLighting(m, LightAmb, LightCol, normal, -lightDir, h);
#endif

	return color;
}
