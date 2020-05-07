#include "common.hlsli"

struct Material {
	float3 Ka;
	float3 Kd;
	float3 Ks;
	float  A;
};

//--------------------------------------------------------------------------------------
// Blinn-Phong Lighting Reflection Model
//--------------------------------------------------------------------------------------
float4 calcBlinnPhongLighting(Material M, float4 LAmb, float4 LColor, float3 N, float3 L, float3 H)
{
	float4 Ia = float4( M.Ka, 1.0f ) * LAmb;
	float4 Id = float4( M.Kd, 1.0f ) * saturate( dot( N, L ) );
	float4 Is = float4( M.Ks, 1.0f ) * pow( saturate( dot( N, H ) ), M.A );

	return Ia + (Id + Is) * LColor;
}

float4 main(in VS_PosColNmlTex input) : SV_TARGET {
	float4 diffuse	= t_diffuse.Sample(t_sampler, input.TexCoord.xy);
	float4 color	= diffuse;

#ifdef USE_DIR_LIGHT
	float4 specular = t_specular.Sample(t_sampler, input.TexCoord.xy);

	Material m;
	m.Ka = AmbientColor.rgb;
	m.Kd = diffuse;//DiffuseColor.rgb;
	m.Ks = (1.0f - specular) * SpecularColor.rgb;
	m.A  = SpecularColor.a;

	float3 normal	= normalize( input.Normal.xyz );
	float3 lightDir	= normalize( LightDir.xyz );
	float3 V = normalize( EyePos - (float3) input.Position );
	float3 h = normalize( -lightDir + V );

	color = calcBlinnPhongLighting(m, LightAmb, LightCol, normal, -lightDir, h);
#endif

	return color;
}
