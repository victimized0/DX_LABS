#include "common.hlsli"

QuadOut FullscreenQuadVS(uint vertexID : SV_VertexID) {
    QuadOut OUT = (QuadOut)0;

    OUT.TexCoord = float2((vertexID << 1) & 2, vertexID & 2);
    OUT.Position = float4(OUT.TexCoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);

    return OUT;
}

float4 BlinnPhongDeferredPS(in QuadOut input) : SV_Target {
	float4 diffuse		= t_diffuse.Sample(t_sampler, input.TexCoord.xy);
	float4 finalColor	= diffuse;

#ifdef USE_LIGHT
	float4 specular	= t_specular.Sample(t_sampler, input.TexCoord.xy);
	float4 normal	= t_normal.Sample(t_sampler, input.TexCoord.xy);
	float3 position	= t_position.Sample(t_sampler, input.TexCoord.xy).xyz;

	Material m;
	m.Ka = 0.1f;
	m.Kd = diffuse.rgb;
	m.Ks = specular.rgb;
	m.A  = specular.a;

	float3 n	= normalize(normal.xyz);
	float3 lDir	= normalize(LightDir.xyz);
	float3 V	= normalize(EyePos - position);
	float3 h	= normalize(-lDir + V);

	finalColor = calcBlinnPhongLighting(m, LightAmb, LightCol, n, -lDir, h);
	if (isnan(finalColor).x) return diffuse;

#endif

    return finalColor;
}

float4 PointLightPS(in VS_PosColNmlTex input) : SV_Target {
	float2 screenTC		= input.Position.xy / gScreenSize;
	float4 diffuse		= t_diffuse.Sample(t_sampler, screenTC.xy);
	float4 finalColor	= 0;

#if USE_LIGHT
	float4 normal	= t_normal.Sample(t_sampler, screenTC.xy);
	float3 position = t_position.Sample(t_sampler, screenTC.xy).xyz;

	float3 n		= normalize(normal.xyz);
	float3 lightDir	= normalize(LightSource.Position - position);
	//float distance	= length(LightSource.Position - position);

	float3 vDist = (LightSource.Position - position) / LightSource.AttRange.w;
	float atten = saturate(1 - dot(vDist, vDist));
	//float atten	= 1 / (LightSource.AttRange.x + LightSource.AttRange.y * distance + LightSource.AttRange.z * (distance * distance));
	float3 diff	= max(dot(n, lightDir), 0.02f) * diffuse.rgb * LightSource.Diffuse.rgb;
	float3 ambi	= diffuse.rgb * LightSource.Ambient.rgb;
	finalColor.rgb = (diff + ambi) * atten;
#endif

	return finalColor;
}
