#pragma pack_matrix(row_major)

cbuffer cbPerInstance : register(b1) {
	float4x4 worldViewProj;
};

struct IA_PosCol {
	float4 Position	: POSITION;
	float4 Color	: COLOR;
};

struct IA_PosNmlTex {
	float4 Position	: POSITION;
	float4 Normal	: NORMAL;
	float2 TexCoord	: TEXCOORD;
};

struct VS_PosCol {
	float4 Position	: SV_POSITION;
	float4 Color	: COLOR;
};

struct VS_PosNmlTex {
	float4 Position	: SV_POSITION;
	float4 Normal	: NORMAL;
	float2 TexCoord	: TEXCOORD;
};
