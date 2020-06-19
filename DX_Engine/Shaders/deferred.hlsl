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

	for (int i = 0; i < POINT_LIGHTS_COUNT; ++i)
	{
		float3 lightDir = normalize(PointLights[i].Position - position);
		float distance = length(PointLights[i].Position - position);
		if (distance < PointLights[i].Range) {
			float attenuation = 1 / (PointLights[i].Attenuation.x + PointLights[i].Attenuation.y * distance + PointLights[i].Attenuation.z * (distance * distance));
			float3 diff = max(dot(n, lightDir), 0.0) * diffuse.rgb * PointLights[i].Diffuse.rgb;
			float3 amb = diffuse.rgb * PointLights[i].Ambient.rgb;
			finalColor.rgb += (diff + amb) * attenuation;
		}
	}
#endif

    return finalColor;
}
