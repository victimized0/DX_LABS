#pragma pack_matrix(row_major)

/////////////////////// DEFINES

#define REG_CB_PER_FRAME		b0
#define REG_CB_PER_INSTANCE		b1
#define REG_CB_PER_MATERIAL		b2
#define REG_CB_PER_LIGHT		b3

#define REG_TEX_DIFFUSE			t0
#define REG_TEX_SPECULAR		t1
#define REG_TEX_NORMAL			t2
#define REG_TEX_POSITION		t3

#define REG_MIP_MIN_MAG_LINEAR	s0

#define POINT_LIGHTS_COUNT		3

/////////////////////// COMMON STRUCTURES

struct Material {
	float3 Ka;
	float3 Kd;
	float3 Ks;
	float  A;
};

struct GBuffer {
	float4 DiffuseAcc	: SV_Target0;
	float4 SpecularAcc	: SV_Target1;
	float4 NormalAcc	: SV_Target2;
	float4 PositionAcc	: SV_Target3;
};

struct PointLight {
	float4	Diffuse;
	float4	Ambient;
	float3	Position;
	float	Range;
	float3	Attenuation;
};

struct QuadOut {
	float4 Position : SV_Position;
	float2 TexCoord : TEXCOORD;
};

/////////////////////// CONSTANT BUFFERS

cbuffer cbPerFrame : register(REG_CB_PER_FRAME) {
	float3	EyePos;
#ifdef USE_LIGHT
	float4	LightCol;
	float4	LightAmb;
	float3  LightDir;
	PointLight PointLights[POINT_LIGHTS_COUNT];
#ifdef USE_BLOOM
	float	BloomThreshold;
#endif // USE_BLOOM
#endif // USE_LIGHT
}

cbuffer cbPerInstance : register(REG_CB_PER_INSTANCE) {
	float4x4 worldViewProj;
	float4x4 worldView;
	float4x4 world;
};

cbuffer cbPerMaterial : register(REG_CB_PER_MATERIAL) {
#ifdef USE_LIGHT
	float4	AmbientColor;
	float4	DiffuseColor;
	float4	SpecularColor;
#endif // USE_LIGHT
}

/////////////////////// TEXTURES

Texture2D t_diffuse		: register(REG_TEX_DIFFUSE);
Texture2D t_specular	: register(REG_TEX_SPECULAR);
Texture2D t_normal		: register(REG_TEX_NORMAL);
Texture2D t_position	: register(REG_TEX_POSITION);
Texture2D t_hdr			: register(REG_TEX_DIFFUSE);
Texture2D t_quadHdr		: register(REG_TEX_DIFFUSE);
Texture2D t_bloom		: register(REG_TEX_SPECULAR);
Texture2D t_src			: register(REG_TEX_DIFFUSE);

/////////////////////// SAMPLER STATES

SamplerState	t_sampler	: register(REG_MIP_MIN_MAG_LINEAR);

/////////////////////// INPUT ASSEMBLER STRUCTURES

struct IA_PosCol {
	float4 Position	: POSITION;
	float4 Color	: COLOR;
};

struct IA_PosNmlTex {
	float4 Position	: POSITION;
	float4 Normal	: NORMAL;
	float2 TexCoord	: TEXCOORD;
};

struct IA_PosColNmlTex {
	float4 Position	: POSITION;
	float4 Color	: COLOR;
	float4 Normal	: NORMAL;
	float2 TexCoord	: TEXCOORD;
};

/////////////////////// VERTEX SHADER OUTPUT STRUCTURES

struct VS_PosCol {
	float4 Position	: SV_POSITION;
	float4 Color	: COLOR;
};

struct VS_PosNmlTex {
	float4 Position	: SV_POSITION;
	float3 Normal	: NORMAL;
	float2 TexCoord	: TEXCOORD;
};

struct VS_PosColNmlTex {
	float4 Position : SV_POSITION;
	float3 WPos		: W_POSITION;
	float4 Color	: COLOR;
	float3 Normal	: NORMAL;
	float2 TexCoord	: TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Blinn-Phong Lighting Reflection Model
//--------------------------------------------------------------------------------------
float4 calcBlinnPhongLighting(in Material M, float4 LAmb, float4 LColor, float3 N, float3 L, float3 H)
{
	float4 Ia = float4(M.Ka, 1.0f) * LAmb;
	float4 Id = float4(M.Kd, 1.0f) * saturate(dot(N, L));
	float4 Is = float4(M.Ks, 1.0f) * pow(saturate(dot(N, H)), M.A);

	return Ia + (Id + Is) * LColor;
}
